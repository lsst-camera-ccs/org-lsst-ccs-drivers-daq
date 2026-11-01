
#include <stdio.h>
#include <unistd.h>

#include "daq/Sensor.hh"
#include "gds/SeriesMetadata.hh"
#include "gds/StateMetadata.hh"
#include "gds/RawStamp.hh"

const char USAGE[] = "usage: gds_rstamp_unpack [-v] <rstamp file(s)>\n";

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
    snprintf(outfilename, sizeof(outfilename), "%s.unpacked", infilename);

    FILE* infile  = fopen(infilename, "rb");
    FILE* outfile = fopen(outfilename, "wb");

    printf("%s:%c", infilename, verbose ? '\n':' '); fflush(stdout);

    GDS::StateMetadata state;
    fread(&state, sizeof(GDS::StateMetadata), 1, infile);
    if(verbose) state.dump(4);
    fwrite(&state, sizeof(GDS::StateMetadata), 1, outfile);

    GDS::SeriesMetadata series;
    fread(&series, sizeof(GDS::SeriesMetadata), 1, infile);
    if(verbose) series.dump(4);
    fwrite(&series, sizeof(GDS::SeriesMetadata), 1, outfile);

    unsigned rstamp_size = GDS::RawStamp::calc_size(series);
    uint8_t* rstamp_buf = new uint8_t[rstamp_size];

    unsigned npix        = GDS::RawStamp::calc_npix(series);
    unsigned ustamp_size = npix*(int)DAQ::Sensor::Segment::NUMOF*sizeof(int32_t);
    uint8_t* ustamp_buf  = new uint8_t[ustamp_size];
    int32_t* ustamps     = (int32_t*)ustamp_buf;

    if(verbose) printf("    ROI %i pixels: RawStamp pixels %i, %i bytes, Unpacked RawStamp %i bytes\n", series.common().pixels(), npix, rstamp_size, ustamp_size);

    int stamps_unpacked = 0;
    while(fread(&state, sizeof(GDS::StateMetadata), 1, infile)) {
      fwrite(&state, sizeof(GDS::StateMetadata), 1, outfile);

      fread(rstamp_buf, rstamp_size, 1, infile);

      ++stamps_unpacked;

      GDS::RawStamp rstamp(rstamp_buf, rstamp_size);

      rstamp.unpack(ustamps);

      fwrite(ustamp_buf, ustamp_size, 1, outfile);
    }

    printf("%*s%i Stamps Unpacked\n", verbose ? 2:0, "", stamps_unpacked);

    fclose(infile);
    fclose(outfile);

    delete [] rstamp_buf;
    delete [] ustamp_buf;
  }

}
