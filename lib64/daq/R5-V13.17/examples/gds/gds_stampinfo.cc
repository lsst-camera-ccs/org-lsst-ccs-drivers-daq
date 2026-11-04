
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "gds/StateMetadata.hh"
#include "gds/SeriesMetadata.hh"
#include "gds/RawStamp.hh"

#include "../emulator/StampFile.hh"
#include "../emulator/FitsFile.hh"

const char USAGE[] = "usage: gds_stampinfo [-v] <stamp, rstamp, or fits file(s)>\n"
                     "  -v  verbose (dump full metadata)\n"
                     "\n"
                     "  Reads .stamp (decoded), .rstamp (raw), and .fits (guider FITS)\n"
                     "  files and prints summary statistics.\n";

enum FileType { FILE_STAMP, FILE_RSTAMP, FILE_FITS };

static FileType _detect_format(const char* filename)
{
  const char* ext = strrchr(filename, '.');
  if(ext && 0 == strcmp(ext, ".rstamp")) return FILE_RSTAMP;
  if(ext && 0 == strcmp(ext, ".fits"))   return FILE_FITS;
  return FILE_STAMP;
}

int main(int argc, char** argv)
{
  bool verbose = false;

  int c;
  while(-1 != (c = getopt(argc, argv, "hv")))
  {
    if('v' == c) verbose = true;
    if('h' == c) {printf(USAGE); return 0;}
  }

  if(optind >= argc) {printf(USAGE); return 0;}

  unsigned files = argc - optind;

  while(files--)
  {
    const char* filename = argv[optind++];

    FileType filetype = _detect_format(filename);

    GDS::Emulator::StampFile* sfile = 0;
    GDS::Emulator::FitsFile*  ffile = 0;

    if(filetype == FILE_FITS)
      ffile = new GDS::Emulator::FitsFile(filename);
    else
    {
      GDS::Emulator::StampFile::Format sfmt =
        (filetype == FILE_RSTAMP) ? GDS::Emulator::StampFile::RAW
                                  : GDS::Emulator::StampFile::DECODED;
      sfile = new GDS::Emulator::StampFile(filename, sfmt);
    }

    bool valid = sfile ? sfile->valid() : ffile->valid();
    if(!valid)
    {
      printf("  ERROR: cannot read file\n");
      delete sfile; delete ffile;
      continue;
    }

    printf("File: %s\n", filename);

    const char* fmt_name = (filetype == FILE_FITS)   ? "FITS (.fits)" :
                           (filetype == FILE_RSTAMP) ? "raw (.rstamp)" : "decoded (.stamp)";
    printf("  Format:      %s\n", fmt_name);

    const GDS::SeriesMetadata& series = sfile ? sfile->series()     : ffile->series();
    const GDS::StateMetadata&  start  = sfile ? sfile->startState() : ffile->startState();
    unsigned decoded_pixels = sfile ? sfile->decoded_pixels() : ffile->decoded_pixels();
    unsigned stamp_sz       = sfile ? sfile->stamp_size()     : ffile->stamp_size();
    unsigned raw_sz         = sfile ? sfile->raw_size()       : ffile->raw_size();

    if(verbose)
    {
      printf("  Start state:\n");
      start.dump(4);
      printf("  Series:\n");
      series.dump(4);
    }

    char loc_buf[64];
    series.location().encode(loc_buf);

    printf("  Sensor:      %s\n", loc_buf);
    printf("  Vendor:      %s\n", series.vendor() == DAQ::Sensor::ITL ? "ITL" :
                                  series.vendor() == DAQ::Sensor::E2V ? "E2V" : "UNKNOWN");
    printf("  ROI:         %u rows x %u cols (integration %u ms)\n",
           series.common().nrows(), series.common().ncols(), series.common().integration());
    printf("  Overscan:    under=%u over=%u overRows=%u\n",
           series.common().underCols(), series.common().overCols(), series.common().overRows());
    printf("  Split ROI:   %s\n", series.splitroi() ? "yes" : "no");
    printf("  Pixels:      %u decoded, %u raw\n",
           decoded_pixels, GDS::RawStamp::calc_npix(series));
    printf("  Stamp size:  %u bytes (file), %u bytes (raw)\n",
           stamp_sz, raw_sz);
    printf("  Sequence:    %u\n", start.sequence());

    if(filetype == FILE_FITS)
      printf("  Stamp count: %u (from header)\n", ffile->stamp_count());

    // Read all records to count stamps and compute cadence
    GDS::StateMetadata state;
    unsigned buf_size = stamp_sz;
    uint8_t* buf = new uint8_t[buf_size];

    uint64_t first_ts = 0;
    uint64_t last_ts  = 0;
    uint64_t sum_delta = 0;
    unsigned count = 0;
    uint64_t prev_ts = 0;
    unsigned pauses  = 0;
    unsigned resumes = 0;
    unsigned series_count = 0;
    bool     has_stop = false;

    while(sfile ? sfile->next(state, buf) : ffile->next(state, buf))
    {
      if(state.type() == GDS::StateMetadata::STAMP)
      {
        uint64_t ts = (uint64_t)state.timestamp();

        if(count == 0)
          first_ts = ts;
        else
          sum_delta += (ts - prev_ts);

        prev_ts = ts;
        last_ts = ts;
        ++count;
      }
      else if(state.type() == GDS::StateMetadata::PAUSE)
      {
        ++pauses;
        if(verbose)
        {
          printf("  ");
          state.dump(2);
        }
      }
      else if(state.type() == GDS::StateMetadata::RESUME)
      {
        ++resumes;
        if(verbose)
        {
          printf("  ");
          state.dump(2);
        }
      }
      else if(state.type() == GDS::StateMetadata::STOP)
      {
        has_stop = true;
        if(verbose)
        {
          printf("  ");
          state.dump(2);
        }
      }
      else if(state.type() == GDS::StateMetadata::START)
      {
        ++series_count;
        // StampFile already updated series/stamp_size for the new START
        unsigned new_buf_size = sfile ? sfile->stamp_size() : ffile->stamp_size();
        if(new_buf_size != buf_size)
        {
          delete[] buf;
          buf_size = new_buf_size;
          buf = new uint8_t[buf_size];
        }
        if(verbose)
        {
          const GDS::SeriesMetadata& new_series = sfile ? sfile->series() : ffile->series();
          printf("  --- New series (sequence %u) ---\n", state.sequence());
          state.dump(2);
          new_series.dump(4);
        }
      }
    }

    printf("  Stamps:      %u\n", count);
    if(pauses)  printf("  Pauses:      %u\n", pauses);
    if(resumes) printf("  Resumes:     %u\n", resumes);
    if(has_stop) printf("  Stop:        yes\n");
    if(series_count) printf("  Series:      %u (additional START records in file)\n", series_count);

    if(count > 1)
    {
      double avg_delta_ms = (double)sum_delta / (double)(count - 1) / 1000000.0;
      double total_s = (double)(last_ts - first_ts) / 1000000000.0;
      printf("  Duration:    %.3f s\n", total_s);
      printf("  Avg cadence: %.3f ms (%.1f Hz)\n", avg_delta_ms, 1000.0 / avg_delta_ms);
    }
    else if(count == 1)
    {
      printf("  Duration:    single stamp\n");
    }
    else
    {
      printf("  Duration:    no stamps in file\n");
    }

    printf("\n");
    delete[] buf;
    delete sfile;
    delete ffile;
  }

  return 0;
}
