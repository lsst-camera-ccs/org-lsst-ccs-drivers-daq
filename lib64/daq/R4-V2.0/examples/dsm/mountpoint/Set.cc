/*
** ++
**  Package:
**	
**
**  Abstract:
**
**  Author:
**      Michael Huffer, SLAC (mehsys@slac.stanford.edu)
**
**  Creation Date:
**	000 - January 09, 2007
**
**  Revision History:
**	None.
**
** --
*/

#include <stdio.h>

#include "dsm/Mountpoint.hh"

#include "Set.hh"

using namespace DSM;
                     
/*
** ++
**
**
** --
*/

static const char COMMAND[] = "set";

MountpointEditor::Set::Set(Interface& interface, Shell& shell) : 
 Command(COMMAND),
 _interface(interface),
 _shell(shell)
 {
 }

/*
** ++
**
**
** --
*/

#define MOUNTPOINT argv[0] 

static const char INVALID_ARGCNT[] = "Only a single argument (which is a NFS mount-point) is allowed for this command\n";
 
void MountpointEditor::Set::process(int argc, const char* argv[], int qualifier)   
 {
 
 if(argc < 1) return;
  
 if(argc > 1) {printf(INVALID_ARGCNT); return;}
  
 DSM::Mountpoint mountpoint(MOUNTPOINT);
 
 DSI::LocationSet missing;
  
 bool success = _interface.set(mountpoint, missing);

 if(!success) _shell._throw(COMMAND, missing);

 return; 
 }

/*
** ++
**
**
** --
*/

static const char HELP[] = "\n" 
                           "  This command assigns all RCEs of the partition to a specified NFS mount-point. The command takes\n"
                           "  a single argument. That argument corresponds to the specification of a valid NFS mount-point.\n"
                           "  A mount-point takes the form: 'IPV4-address:/local-directory/'. Where:\n"
                           "    IPV4-Address    - is a valid IPV4 address of the REMOTE node servicing NFS requests.\n"
                           "    local-directory - Is the (RCE) LOCAL, (absolute) file directory at which to mount. Note\n"
                           "                      the directory is bracketed by slashes (/) at both beginning and end.\n" 
                           "  For example: '192.168.100.1:/mnt/nfs/lsst-daq/' might form a typical mount-point assignment.\n"
                           "  Caution: If the mount-point specification is invalid the partition could be left in an un-bootable state.\n"
                           "  Mount-point assignments are transitory until made persistant. Assignments can only be made persistant\n" 
                           "  by restarting the partition. See the <restart> command to restart a partition. To see current\n"
                           "  assignments see the <dump> command. If an assignment is pending (that is, it has been set, but the\n" 
                           "  partition has not yet been restarted), its current assignment will be marked <dirty>. To see any\n" 
                           "  pending assignments see the <dump/pending> commmand.\n"   
                           "\n";
                           
void MountpointEditor::Set::help(int argc, const char** argv, int qualifier)   
 {
 
 printf(HELP);

 return;
 }

