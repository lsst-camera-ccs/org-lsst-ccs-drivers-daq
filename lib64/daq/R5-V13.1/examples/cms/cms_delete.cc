
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <csignal>
#include <string.h>
#include <zlib.h>

#include "daq/LocationSet.hh"
#include "daq/ScienceSet.hh"
#include "daq/GuidingSet.hh"
#include "cms/Camera.hh"
#include "cms/Exception.hh"
#include "ims/Path.hh"
#include "ims/Image.hh"

#define PROGRAM    argv[0]
#define PARTITION  argv[1]
#define IMAGE_BASE argv[2]
#define START      argv[3]
#define REPEATS    argv[4]

static const char USAGE[] = "usage: cms_trigger <partition> <image base name> <starting index> <number of images> [DAQ Locations]\n";

static const char folder[]="";
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

static const char SUCCESS[]        = "Triggered using opcode %d the image named %s at %s\n";
static const char FAILED[]         = "Failed to trigger using opcode %d the image to be named %s (%s)\n";
static const char NOT_VALID[]      = "%s is not a valid image specifier (first argument must be of the form: 'folder-name/image-name')\n";
static const char LOOKUP_ERROR[] = "Cannot find '%s in the storage catalog\n";
static const char OPEN_ERROR[]   = "Cannot locate the metadata for '%s\n";

int main(int argc, char* argv[])
{

  if(argc < 5) {printf(USAGE); DAQ::LocationSet::usage(); return EXIT_SUCCESS;}
  
  struct sigaction sigIntHandler;
  sigIntHandler.sa_handler = quitIt;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;
  sigaction(SIGINT, &sigIntHandler, NULL);
  
  IMS::Store store(PARTITION);

  unsigned repeats = atol(REPEATS);
  unsigned image_number = atol(START);
  char image_name[64];
  char* image = &image_name[0];
  unsigned iterations = 0;

  while(!doExit)
  {
    sprintf(image_name, "%s%06i", IMAGE_BASE, image_number);

    IMS::Path path(image, folder);
     
    if(!path) {printf(NOT_VALID, image_name); return -1;}

    IMS::Id id = store.catalog.lookup(path.image(), path.folder());

    if(!id) {printf(LOOKUP_ERROR, image_name); return -1;}

    IMS::Image image(id, store);

    if(!image) {printf(OPEN_ERROR, image_name); return -1;}

    int error = image.remove();
    printf("Removing image %s, returned %i\n", image_name, error);

    ++image_number;
    if(++iterations==repeats) break;
  }
  
  return(EXIT_SUCCESS);
}

