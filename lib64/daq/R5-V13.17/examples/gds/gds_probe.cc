
#include <stdio.h>
#include <stdlib.h>

#include "dvi/Version.hh"
#include "dsm/Client.hh"
#include "dsm/Services.hh"
#include "dsi/Set.hh"
#include "dsi/Location.hh"
#include "dsi/Platform.hh"
#include "net/ipv4/Address.hh"
#include "daq/Location.hh"

static const char USAGE[] = "usage: gds_probe <partition>\n";

#define PARTITION argv[1]

static const char GDS_HEADER[] =
  " Slot ( Index  )     Address         Platform                    Build Information                    \n"
  " -------------- --------------- ---------------- ----------------------------------------------------\n";

static const char GDR_HEADER[] =
  " Server ( Index  ) Location ( Index  )     Address         Platform                    Build Information                    \n"
  " ----------------- ------------------- --------------- ---------------- ----------------------------------------------------\n";

int main(int argc, char** argv)
{
  if(argc != 2) {printf(USAGE); return EXIT_SUCCESS;}

  // Section 1: local client library version
  DVI::Version version;
  printf("\n");
  version.header();
  version.dump();

  // Section 2: GDS System Service (control tier)
  // Server column is the raw slot index
  {
    DSM::Client gds(DSM::Services::Id::GDS, PARTITION);

    IPV4::Address addresses[DSI::Set::SIZE];
    DVI::Version  versions[DSI::Set::SIZE];
    DSI::Platform platforms[DSI::Set::SIZE];

    gds.probe(addresses);
    gds.probe(versions);
    gds.probe(platforms);

    printf("\nGDS System:\n");
    printf(GDS_HEADER);

    DSI::Set remaining(gds.partition_set());

    unsigned total  = 0;
    unsigned server = remaining.remove();

    while(server != DSI::Set::SIZE)
    {
      char buffer[32];

      const IPV4::Address* address  = &addresses[server];
      const DVI::Version*  ver      = &versions[server];
      const DSI::Platform* platform = &platforms[server];

      printf(" %-4d (0x%02X %3u) %-15s %-16s %s [%08X] (%s), %-20s\n",
             server, server, server,
             address->presentation(buffer),
             platform->name(),
             ver->tag(), ver->hash(), ver->dirty() ? "dirty" : "clean", ver->buildTime.decode()
             );

      total++;
      server = remaining.remove();
    }

    printf("  total = %d\n", total);
  }

  // Section 3: GDR Readout Service (data tier)
  // Server column uses DAQ::Location — bay/board format
  {
    DSM::Client gdr(DSM::Services::Id::GDR, PARTITION);

    DSI::Location locations[DSI::Set::SIZE];
    IPV4::Address addresses[DSI::Set::SIZE];
    DVI::Version  versions[DSI::Set::SIZE];
    DSI::Platform platforms[DSI::Set::SIZE];

    gdr.probe(locations);
    gdr.probe(addresses);
    gdr.probe(versions);
    gdr.probe(platforms);

    printf("\nGDR Readout:\n");
    printf(GDR_HEADER);

    DSI::Set remaining(gdr.partition_set());

    unsigned total  = 0;
    unsigned server = remaining.remove();

    while(server != DSI::Set::SIZE)
    {
      char buffer1[16];
      char buffer2[8];
      char buffer3[32];

      DAQ::Location        daq_loc((uint8_t)server);
      const DSI::Location* location = &locations[server];
      unsigned             index    = location->index();
      const IPV4::Address* address  = &addresses[server];
      const DVI::Version*  ver      = &versions[server];
      const DSI::Platform* platform = &platforms[server];

      printf(" %-6s (0x%02X %3u) %-8s (0x%02X %3u) %-15s %-16s %s [%08X] (%s), %-20s\n",
             daq_loc.encode(buffer1), server, server,
             location->encode(buffer2), index, index,
             address->presentation(buffer3),
             platform->name(),
             ver->tag(), ver->hash(), ver->dirty() ? "dirty" : "clean", ver->buildTime.decode()
             );

      total++;
      server = remaining.remove();
    }

    printf("  total = %d\n", total);
  }

  return EXIT_SUCCESS;
}
