/*
** ++
**  Package:
**	
**
**  Abstract:      
**
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
   
#include "ims/Store.hh"
#include "ims/Shell.hh"

#define PROGRAM   argv[0]
#define PARTITION argv[1]
#define INTERFACE argv[2]

using namespace IMS;

static const char DEFAULT[] = "";
static const char NONE[]    = "";

static const char FOLDER[] = "$emulation";

static const char ANNOUNCE[] = "  Manage the IMS store. Input to the utility requires either zero, one or two arguments. Where:\n" 
                               "  If two arguments - The first argument corresponds to the name of of a valid partition and the second argument to the name of a valid network interface.\n" 
                               "  If one argument  - The argument corresponds to the name of a valid partition and defaults the network interface.\n"
                               "  If no arguments  - Defaults to the set of RCEs not in *any* partition and defaults the network interface.\n";

int main(int argc, char** argv)
 {

 --argc;
 
 if(!argc) {printf(ANNOUNCE); return EXIT_SUCCESS;}
 
 const char* network;
 const char* partition;
 
 switch(argc) {
  case 0: network = DEFAULT;   partition = NONE;      break;
  case 1: network = DEFAULT;   partition = PARTITION; break;
  case 2: network = INTERFACE; partition = PARTITION; break;
  }
  
 Store store(partition, network);
  
 Shell shell(store, FOLDER);
  
 shell.start();
 
 return EXIT_SUCCESS;
 }

