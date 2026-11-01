
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "gds/StateMetadata.hh"
#include "gds/SeriesMetadata.hh"
#include "gds/RawStamp.hh"
#include "gds/Stamp.hh"

#include "../emulator/StampFile.hh"
#include "../emulator/FitsFile.hh"
#include "../emulator/Encoder.hh"

const char USAGE[] =
  "usage: gds_stamp_roundtrip [-v] <file(s)>\n"
  "       gds_stamp_roundtrip -x <stamp file> <rstamp file>\n"
  "\n"
  "  Default mode: round-trip test. Reads a .stamp or .fits file, encodes\n"
  "  each stamp to raw 18-bit packed format, decodes back, and verifies\n"
  "  the output matches the input pixel-by-pixel.\n"
  "\n"
  "  -x mode: cross-format validation. Reads a .stamp and matching .rstamp\n"
  "  file, decodes each raw stamp, and compares against decoded pixels.\n"
  "\n"
  "  -v  verbose (print per-stamp pass/fail)\n";

static int _roundtrip(const char* filename, bool verbose);
static int _crossformat(const char* stamp_file, const char* rstamp_file, bool verbose);

int main(int argc, char** argv)
{
  bool verbose = false;
  bool crossformat = false;

  int c;
  while(-1 != (c = getopt(argc, argv, "hvx")))
  {
    if('v' == c) verbose = true;
    if('x' == c) crossformat = true;
    if('h' == c) {printf(USAGE); return 0;}
  }

  if(optind >= argc) {printf(USAGE); return 1;}

  if(crossformat)
  {
    if(argc - optind < 2) {printf("ERROR: -x requires <stamp file> <rstamp file>\n"); return 1;}
    return _crossformat(argv[optind], argv[optind+1], verbose);
  }

  int result = 0;
  unsigned files = argc - optind;
  while(files--)
  {
    result |= _roundtrip(argv[optind++], verbose);
  }
  return result;
}

static bool _is_fits(const char* filename)
{
  const char* ext = strrchr(filename, '.');
  return ext && 0 == strcmp(ext, ".fits");
}

/*
** ++
**
** Round-trip test: decode → encode → decode, compare.
** Works with both .stamp and .fits files.
**
** --
*/

static int _roundtrip(const char* filename, bool verbose)
{
  printf("Round-trip: %s\n", filename);

  // Open file — one of these will be non-null
  GDS::Emulator::StampFile* sfile = 0;
  GDS::Emulator::FitsFile*  ffile = 0;

  if(_is_fits(filename))
    ffile = new GDS::Emulator::FitsFile(filename);
  else
    sfile = new GDS::Emulator::StampFile(filename, GDS::Emulator::StampFile::DECODED);

  bool valid = sfile ? sfile->valid() : ffile->valid();
  if(!valid) { delete sfile; delete ffile; return 1; }

  const GDS::SeriesMetadata& series = sfile ? sfile->series() : ffile->series();
  GDS::Emulator::Encoder* encoder = new GDS::Emulator::Encoder(series);

  unsigned dpix = series.common().pixels();
  unsigned dsize = dpix * sizeof(int32_t);
  unsigned rsize = encoder->raw_size();

  uint8_t* decoded_buf = new uint8_t[dsize];
  uint8_t* redecoded_buf = new uint8_t[dsize];

  GDS::StateMetadata state;
  unsigned passed = 0;
  unsigned failed = 0;
  unsigned stamp_num = 0;

  auto next_record = [&](GDS::StateMetadata& s, uint8_t* buf) -> bool {
    return sfile ? sfile->next(s, buf) : ffile->next(s, buf);
  };

  while(next_record(state, decoded_buf))
  {
    // Skip non-stamp records (PAUSE, RESUME, STOP, START)
    if(state.type() != GDS::StateMetadata::STAMP) continue;

    ++stamp_num;

    // Handle mid-file START: recreate encoder with new series
    // (only possible with StampFile; FitsFile has a single series)
    if(sfile && sfile->stamp_size() != dsize)
    {
      dsize = sfile->stamp_size();
      dpix  = sfile->series().common().pixels();
      rsize = GDS::RawStamp::calc_size(sfile->series());
      delete[] decoded_buf;
      delete[] redecoded_buf;
      decoded_buf   = new uint8_t[dsize];
      redecoded_buf = new uint8_t[dsize];
      delete encoder;
      encoder = new GDS::Emulator::Encoder(sfile->series());
    }

    // Encode decoded → raw
    const uint8_t* raw = encoder->encode((const int32_t*)decoded_buf);

    // Decode raw → redecoded
    GDS::RawStamp rstamp((uint8_t*)raw, rsize);
    GDS::Stamp redecoded(redecoded_buf, dsize);
    const GDS::SeriesMetadata& cur_series = sfile ? sfile->series() : ffile->series();
    rstamp.decode(redecoded, cur_series);

    // Compare
    const int32_t* orig = (const int32_t*)decoded_buf;
    const int32_t* redc = redecoded.pixels();
    bool match = true;
    unsigned first_diff = 0;

    for(unsigned i = 0; i < dpix; ++i)
    {
      if(orig[i] != redc[i])
      {
        match = false;
        first_diff = i;
        break;
      }
    }

    if(match)
    {
      ++passed;
      if(verbose) printf("  Stamp %3u: PASS\n", stamp_num);
    }
    else
    {
      ++failed;
      printf("  Stamp %3u: FAIL - pixel %u: expected %d, got %d\n",
             stamp_num, first_diff, orig[first_diff], redc[first_diff]);
    }
  }

  printf("  Result: %u passed, %u failed (of %u stamps)\n\n",
         passed, failed, passed + failed);

  delete encoder;
  delete[] decoded_buf;
  delete[] redecoded_buf;
  delete sfile;
  delete ffile;

  return failed ? 1 : 0;
}

/*
** ++
**
** Cross-format: decode .rstamp, compare against .stamp pixels.
**
** --
*/

static int _crossformat(const char* stamp_filename, const char* rstamp_filename, bool verbose)
{
  printf("Cross-format: %s vs %s\n", stamp_filename, rstamp_filename);

  GDS::Emulator::StampFile dfile(stamp_filename,  GDS::Emulator::StampFile::DECODED);
  GDS::Emulator::StampFile rfile(rstamp_filename, GDS::Emulator::StampFile::RAW);

  if(!dfile.valid() || !rfile.valid()) return 1;

  const GDS::SeriesMetadata& series = dfile.series();
  unsigned dpix  = series.common().pixels();
  unsigned dsize = dpix * sizeof(int32_t);
  unsigned rsize = rfile.stamp_size();

  uint8_t* decoded_buf   = new uint8_t[dsize];
  uint8_t* raw_buf       = new uint8_t[rsize];
  uint8_t* redecoded_buf = new uint8_t[dsize];

  GDS::StateMetadata dstate, rstate;
  unsigned passed = 0;
  unsigned failed = 0;

  // Advance decoded file to next STAMP record
  auto next_stamp = [](GDS::Emulator::StampFile& f, GDS::StateMetadata& s, uint8_t* buf) -> bool {
    while(f.next(s, buf))
      if(s.type() == GDS::StateMetadata::STAMP) return true;
    return false;
  };

  while(next_stamp(dfile, dstate, decoded_buf) && next_stamp(rfile, rstate, raw_buf))
  {
    unsigned stamp_num = dfile.stamps_read();

    // Decode raw stamp
    GDS::RawStamp rstamp(raw_buf, rsize);
    GDS::Stamp redecoded(redecoded_buf, dsize);
    rstamp.decode(redecoded, dfile.series());

    // Compare
    const int32_t* orig = (const int32_t*)decoded_buf;
    const int32_t* redc = redecoded.pixels();
    bool match = true;
    unsigned first_diff = 0;

    for(unsigned i = 0; i < dpix; ++i)
    {
      if(orig[i] != redc[i])
      {
        match = false;
        first_diff = i;
        break;
      }
    }

    if(match)
    {
      ++passed;
      if(verbose) printf("  Stamp %3u: PASS\n", stamp_num);
    }
    else
    {
      ++failed;
      printf("  Stamp %3u: FAIL - pixel %u: expected %d, got %d\n",
             stamp_num, first_diff, orig[first_diff], redc[first_diff]);
    }
  }

  printf("  Result: %u passed, %u failed (of %u stamps)\n\n",
         passed, failed, passed + failed);

  delete[] decoded_buf;
  delete[] raw_buf;
  delete[] redecoded_buf;

  return failed ? 1 : 0;
}
