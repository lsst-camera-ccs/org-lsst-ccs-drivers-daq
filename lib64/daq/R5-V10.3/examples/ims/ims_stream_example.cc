/*
** ++
**  Package:
**
** DSId	
**
**  Abstract:

 Start a DSI Daemon (see DSId::Server), whose scope is determined by a backing file 
 created using the dsid_configure utility. The started daemon will listen on a specified
 network interface. From the command line this utility is invoked with two (2) arguments:
 
  - The first argument is a string which specifies the network interface to which the 
    started daemon will listen on for work requests.  
  - The second argument is a string which specifies the backing file for the daemon. The  
    string is interpreted as a path name corresponding to the backing file.  

 Assuming both arguments are provided, on success the utility starts the daemon with a scope
 provided by the backing file and on failure sends (to standard output) an appropriate error
 message. 
 
**  Author:
**      Michael Huffer, SLAC <mehsys@slac.stanford.edu>
**
**  Creation Date:
**	000 - , 2007
**
**  Revision History:
**	None.
**
** --
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "ims/Stream.hh"
#include "ims/Store.hh"
#include "ims/Image.hh"
#include "ims/Barrier.hh"

#define PROGRAM   argv[0]
#define PARTITION argv[1]

#define IMAGE_TIMOUT  (15 * 1000 * 1000) // 15 seconds expressed in 1us tics... 
#define SOURCE_TIMOUT ( 3 * 1000 * 1000) //  3 seconds expressed in 1us tics... 

using namespace IMS;

/*
** ++
**
**
** --
*/

int main(int argc, char* argv[])
 {

 Store store(PARTITION);
 
 Stream stream(store);                     // or with time-out... Stream stream(store, SOURCE_TIMOUT);
 
 Image image(store, stream, IMAGE_TIMOUT); // or without time-out... Image image(store, stream);
 
 if(!image) return EXIT_FAILURE;           // timed-out...
 
 Barrier barrier(image);
 
 barrier.block(stream);
  
 Image::title(); image.synopsis();
  
 return(EXIT_SUCCESS);
 }
