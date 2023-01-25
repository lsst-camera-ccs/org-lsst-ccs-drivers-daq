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

#include "ims/Store.hh"
#include "emu/PlayList.hh"
#include "Shell.hh"

#define PROGRAM   argv[0]
#define PARTITION argv[1]
#define FOLDER    argv[2]
#define PLAYLIST  argv[3]

static const char ANNOUNCE[] = "\n"
                               "  The Emulation Editor is used to manage a playlist. Input to the editor requires either two or three arguments. Where:\n" 
                               "  If two arguments   - The first argument corresponds to the name of a valid partition and the second the name of the default folder.\n"
                               "  If three arguments - The first argument corresponds to the name of a valid partition, the second the name of the default folder and\n"
                               "                       the third is a valid directory path pointing to the backing faile for a playlist. Note: The playlist is opened\n"
                               "                       if found and created if not.\n"
                               "\n";

int main(int argc, char** argv)
 {

 if(!--argc) {printf(ANNOUNCE); return EXIT_SUCCESS;}
 
 IMS::Store store(PARTITION); 

 EMU::PlayList* playlist = argc >=3 ? new EMU::PlayList(PLAYLIST) : (EMU::PlayList*)0;
   
 EMU::Shell shell(store, FOLDER, playlist);
   
 shell.start();

 return EXIT_SUCCESS;
 }


