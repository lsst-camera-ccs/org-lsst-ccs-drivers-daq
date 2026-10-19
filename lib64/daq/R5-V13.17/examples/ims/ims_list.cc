#include <stdio.h>
#include <stdlib.h>

#include "ims/Store.hh"
#include "List.hh"

static const char USAGE[] = "usage: ims_list partition [folder]\n";

#define PARTITION argv[1]
#define FOLDER    argv[2]

int main(int argc, char** argv)
{
  if(argc < 2) {printf(USAGE); return EXIT_SUCCESS;}

  IMS::Store store(PARTITION);
  IMS::Editor::List list(store);
  list.process(argc-2, (const char**)&FOLDER, 0);
  return EXIT_SUCCESS;
}
