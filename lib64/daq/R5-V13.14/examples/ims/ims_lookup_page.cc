
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "daq/LocationSet.hh"
#include "ims/Image.hh"
#include "ims/Bucket.hh"

#define PROGRAM    argv[0]
#define PARTITION  argv[1]
#define FOLDER     argv[2]
#define IMAGE      argv[3]
#define SOURCE     argv[4]
#define OFFSET     argv[5]

#define INVALID        -1 // invalid opcode...

static const char USAGE[] = "usage: find_page <partition> <folder> <image name> <source> <offset>\n";

/*
** ++
**
**
** --
*/
static const char NOT_VALID[]      = "Couldn't find metadata for image %s/%s\n"; 

#include <zlib.h>
#include <string.h>

int main(int argc, char* argv[])
{
  
  if(argc < 3) {printf(USAGE); return EXIT_SUCCESS;}
 
  IMS::Store store(PARTITION);

  IMS::Id ims_id = store.catalog.lookup(IMAGE, FOLDER);
  IMS::Image image(ims_id, store);

  if(!image) {printf(NOT_VALID, FOLDER, IMAGE); return -1;}

  DAQ::Location loc(SOURCE);

  XDS::Id xds_id(ims_id, XDS::Id::Space::IMS, IMS::Bucket::Type::DATA, loc.index());
  
  uint64_t offset = atoll(OFFSET);
  XDS::Page page(xds_id, offset);
  
  uint64_t page_id = page.id();
  unsigned server = store.resolve(page_id);
  IPV4::Address address = store.lookup(page);
  
  printf("ims_id %llX, xds_id %llX, page_id %llX, server %X\n", (long long unsigned)ims_id.value(), (long long unsigned)xds_id.value(), (long long unsigned)page_id, server);
  address.print();
  
  return(EXIT_SUCCESS);
}

