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

#include "dsm/Releasepoint.hh"

#include "Set.hh"

using namespace DSM;
                     
/*
** ++
**
**
** --
*/

static const char COMMAND[] = "set";

ReleasepointEditor::Set::Set(Interface& interface, Shell& shell) : 
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

#define RELEASEPOINT argv[0] 

static const char INVALID_ARGCNT[] = "Only a single argument (which is a release-point) is allowed for this command\n";
 
void ReleasepointEditor::Set::process(int argc, const char* argv[], int qualifier)   
 {
 
 if(argc < 1) return;
 
 if(argc > 1) {printf(INVALID_ARGCNT); return;}
  
 DSM::Releasepoint releasepoint(RELEASEPOINT);
 
 DSI::LocationSet missing;
  
 bool success = _interface.set(releasepoint, missing);

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
                           "  This command assigns all RCEs of the partition to a specified release directory. The command takes\n"
                           "  a single argument. That argument corresponds to the specification of an (RCE) absolute file path\n"
                           "  pointing to a directory containing a valid DAQ release. it takes the form '/directory/'. Note that\n"
                           "  the directory specification is bracketed by slashes (/) at both beginning and end. For example:\n" 
                           "  '/mnt/nfs/users/mike/releases/rtems/loadable/' might form a typical release-point assignment.\n"
                           "  Caution: If either release-point specification or what its points to does not contain a valid release\n" 
                           "  the partitions's RCEs could be left in an un-bootable state.\n"
                           "  Release-point assignments are transitory until made persistant. Assignments can only be made persistant\n" 
                           "  by restarting the partition. See the <restart> command to restart a partition. To see current\n"
                           "  assignments see the <dump> command. If an assignment is pending (that is, it has been set, but the\n" 
                           "  partition has not yet been restarted), its current assignment will be marked <dirty>. To see any\n" 
                           "  pending assignments see the <dump/pending> commmand.\n"   
                           "\n";

void ReleasepointEditor::Set::help(int argc, const char** argv, int qualifier)   
 {
 
 printf(HELP);

 return;
 }

