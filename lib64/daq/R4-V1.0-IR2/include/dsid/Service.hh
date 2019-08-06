
/*
**  Package:
**	
**
**  Abstract:
**
**  Author:
**      Michael Huffer, SLAC (mehsys@slac.stanford.edu)
**
**  Creation Date:
**	    000 - April 06, 2011
**
**  Revision History:
**	    None.
**
** --
*/
 
#ifndef DSID_SERVICE
#define DSID_SERVICE

#include "dsi/Frame.hh"
#include "dsi/Service.hh"
#include "dsid/Partitions.hh"

namespace DSID {

class Service : public DSI::Service {
public:
  Service(const char* interface, const char* directory);
public:  
  ~Service();
public:   
  void M04(DSI::Frame&, const void* input, int size); // Create partition
  void M05(DSI::Frame&, const void* input, int size); // Return ID (Lookup by name)  
  void M06(DSI::Frame&, const void* input, int size); // Return Name (Lookup by ID)
  void M07(DSI::Frame&, const void* input, int size); // Return (as a bitlist) all IDs
  void M08(DSI::Frame&, const void* input, int size); // Delete partition
private:
  Partitions _partitions;
}; 

}

#endif

