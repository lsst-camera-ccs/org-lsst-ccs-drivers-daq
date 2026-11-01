#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ims/Store.hh"
#include "BadPixelCount.hh"

static const char USAGE[] =
  "usage: ims_badpixel partition [-d] <folder/image> [low high [max]]\n"
  "\n"
  "  Scan an image for pixels outside the good range [low, high] (default [10000, 240000]).\n"
  "  Image must be specified as folder-name/image-name.\n"
  "\n"
  "  -d  Dump bad stripes with context. Optional 'max' limits stripes shown\n"
  "      (default 100, 0 for unlimited).\n";

#define PARTITION argv[1]

int main(int argc, char** argv)
{
  if(argc < 3) {printf(USAGE); return EXIT_SUCCESS;}

  int qualifier = 0;
  int arg = 2;

  if(0 == strncmp(argv[arg], "-d", 2))
  {
    qualifier = 2;
    arg++;
  }

  int pargc = argc - arg;
  const char** pargv = (const char**)&argv[arg];

  IMS::Store store(PARTITION);
  IMS::Editor::BadPixelCount badpixel(store, "");
  badpixel.process(pargc, pargv, qualifier);
  return EXIT_SUCCESS;
}
