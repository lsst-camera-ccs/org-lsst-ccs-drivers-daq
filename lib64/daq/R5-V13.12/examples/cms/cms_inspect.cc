
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <csignal>

#include "daq/LocationSet.hh"
#include "daq/ScienceSet.hh"
#include "daq/WavefrontSet.hh"
#include "daq/GuidingSet.hh"
#include "rms/InstructionList.hh"
#include "cms/Camera.hh"
#include "cms/Exception.hh"
#include "ims/Path.hh"
#include "ims/Image.hh"
#include "ims/Stream.hh"
#include "ims/Barrier.hh"
#include "ims/../src/editor/Inspector.hh"

#define INVALID        -1 // invalid opcode...

static const char USAGE[] = "usage: cms_inspect <partition> <folder> <image base> <start idx> <repeats>\n";

static bool doExit = false;

void quitIt(int s)
{
  printf("\n");
  if(doExit)
  {
    printf("\n");
    exit(1);
  }
  doExit=true;
}

/*
** ++
**
**
** --
*/
static const char NOT_VALID[]      = "ERROR - Couldn't find metadata for image %s/%s\n"; 

#include <string.h>

int main(int argc, char* argv[])
{
  bool verbose = false;

  if(argc < 6) {printf(USAGE); return EXIT_SUCCESS;}

  int arg = 0;
  char*  PARTITION  =  argv[++arg];
  char*  FOLDER     =  argv[++arg];
  if(0 == strncmp(FOLDER,"-v", 2))
  {
    verbose = true;
    FOLDER = argv[++arg];
  }
  char*  IMAGE_BASE =  argv[++arg];
  char*  START      =  argv[++arg];
  char*  REPEATS    =  argv[++arg];

  struct sigaction sigIntHandler;
  sigIntHandler.sa_handler = quitIt;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;
  sigaction(SIGINT, &sigIntHandler, NULL);
 
  IMS::Store store(PARTITION);
  CMS::Camera camera(store);

  unsigned repeats = atol(REPEATS);
  unsigned image_number = atol(START);
  char image_name[64];
  unsigned iterations = 0;

  while(!doExit)
  {
    sprintf(image_name, "%s%06i", IMAGE_BASE, image_number);

    IMS::Id id = store.catalog.lookup(image_name, FOLDER);
    IMS::Image image(id, store);
    
    if(!image) 
    {
      printf(NOT_VALID, FOLDER, image_name);
    }
    else
    {
      if(verbose) 
      {
        IMS::Image::title();
        image.synopsis();
      }
      else
      {
        char buffer[512];
        image.synopsis(buffer);
        printf("%s -", buffer);
      }

      IMS::Editor::Inspector decoder(image, verbose);
      decoder.run();
      decoder.summary();
    }
    ++image_number;
    if(++iterations==repeats) break;
  }

  return(EXIT_SUCCESS);
}
