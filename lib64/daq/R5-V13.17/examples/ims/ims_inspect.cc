#include <stdio.h>
#include <stdlib.h>

#include "ims/Store.hh"
#include "Inspect.hh"

static const char USAGE[] = "usage: ims_inspect partition <folder/image>\n";

#define PARTITION argv[1]
#define IMAGE     argv[2]

int main(int argc, char** argv)
{
  if(argc < 3) {printf(USAGE); return EXIT_SUCCESS;}

  IMS::Store store(PARTITION);
  IMS::Editor::Inspect inspect(store, "");
  inspect.process(argc-2, (const char**)&IMAGE, 0);
  return EXIT_SUCCESS;
}
