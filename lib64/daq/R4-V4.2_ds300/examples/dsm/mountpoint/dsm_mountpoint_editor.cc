/*
** ++
**  Package:
**	
**
**  Abstract:

 Shutdown a pre-existing, running DSI Daemon (see DSId::Server). From the command line 
 this utility is invoked with one (1) argument:

- The only argument is a small, (decimal) number enumerating a scope. For each scope there 
   exists one and only one DSId::Daemon. Therefore, from this argument the address of the 
   appropriate daemon is inferred. Valid scope numbers range from zero (0) to thirty-one (31). 
   This argument MUST be supplied. 
   
 Assuming one valid argument is provided, on success the utility shuts down the corresponding 
 daemon and on failure sends (to standard output) an appropriate error message.            

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
   
#include "dsm/Interface.hh"
#include "Shell.hh"

#define PROGRAM   argv[0]
#define PARTITION argv[1]
#define INTERFACE argv[2]

const char* _partition(const char* partition);

static const char DEFAULT[] = "";

static const char ANNOUNCE[] = "\n"
                               "  Enter the Mountpoint editor. This editor manages the NFS mount-point for the RCES of a partition. Input to the utility requires either one or two arguments. Where:\n" 
                               "  If one argument  - The argument corresponds to the name of a valid partition whose RCEs are to be edited and defaults the network interface.\n"
                               "  If two arguments - The first argument is again the name of of a valid partition while the second argument is the name of a valid network interface.\n" 
                               "  Note: If the first argument is an ellipsis (three consecutive dots or ...), the set of RCEs NOT in any partition are edited.\n"; 

int main(int argc, char** argv)
 {

 using namespace DSM;

 --argc;
 
 if(!argc) {printf(ANNOUNCE); return EXIT_SUCCESS;}
   
 const char* partition = _partition(PARTITION);
 const char* network   = argc != 1 ? INTERFACE : DEFAULT;
 
 unsigned pid = Interface::lookup(partition, network);
 
 Interface interface(pid, network);
    
 MountpointEditor::Shell shell(interface);
   
 shell.start();
 
 return EXIT_SUCCESS;
 }
 
/*
** ++
**
** --
*/

static const char NONE[] = "";

const char* _partition(const char* partition)
 {
 
 const char* none = partition;
 
 if(*none++ != '.') return partition;
 if(*none++ != '.') return partition;
 if(*none++ != '.') return partition;
 
 return *none ? partition : NONE;
 }
