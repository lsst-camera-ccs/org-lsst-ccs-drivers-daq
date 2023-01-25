
#include <stdio.h>
#include <unistd.h>

#include "daq/Sensor.hh"
#include "gds/SeriesMetadata.hh"
#include "gds/RawStamp.hh"
#include "gds/Stamp.hh"

const char USAGE[] = "usage: gds_rstamp_decode [-v] <rstamp file(s)>\n";

int main(int argc, char** argv)
{

  bool verbose = false;

  int c;
  while(-1 != (c = getopt(argc, argv, "v")))
    if('v' == c) verbose = true;
  
  if(optind<1) {printf(USAGE); return 0;}

  unsigned files = argc-optind;

  while(files--)
  {
    char* infilename = argv[optind++];

    char outfilename[1024];
    snprintf(outfilename, sizeof(outfilename), "%s.decoded", infilename);

    FILE* infile  = fopen(infilename, "rb");
    FILE* outfile = fopen(outfilename, "wb");

    GDS::SeriesMetadata series;
    fread(&series, sizeof(GDS::SeriesMetadata), 1, infile);

    printf("%s:%c", infilename, verbose ? '\n':' '); fflush(stdout);
    if(verbose) series.dump(4);

    fwrite(&series, sizeof(GDS::SeriesMetadata), 1, outfile);

    int rpix = GDS::RawStamp::calc_npix(series);
    int dpix = series.common().pixels();

    unsigned rstamp_size = GDS::RawStamp::calc_size(series);
    uint8_t* rstamp_buf = new uint8_t[rstamp_size];

    unsigned dstamp_size = dpix*(int)sizeof(int32_t);
    uint8_t* dstamp_buf = new uint8_t[dstamp_size];
    int32_t* dstamps = (int32_t*)dstamp_buf;

    if(verbose) printf("    RawStamp %i pixels, %i bytes, Decoded Stamp %i pixels, %i bytes\n", rpix, rstamp_size, dpix, dstamp_size);

    int stamps_decoded = 0;
    while(fread(rstamp_buf, rstamp_size, 1, infile)) {

      ++stamps_decoded;

      GDS::RawStamp rstamp(rstamp_buf, rstamp_size);

      GDS::Stamp dstamp(dstamp_buf, dstamp_size);

      rstamp.decode(dstamp, series);

      fwrite(dstamps, dstamp_size, 1, outfile);
    }

    printf("%*s%i Stamps Decoded\n", verbose ? 2:0, "", stamps_decoded);

    fclose(infile);
    fclose(outfile);

    delete [] rstamp_buf;
    delete [] dstamp_buf;
  }
}
