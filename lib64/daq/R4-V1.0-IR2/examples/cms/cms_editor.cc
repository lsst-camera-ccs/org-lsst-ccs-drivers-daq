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
#include "cms/Camera.hh"
#include "Shell.hh"

#define PROGRAM   argv[0]
#define PARTITION argv[1]
#define FOLDER    argv[2]

static const char ANNOUNCE[] = "Enter the CMS Camera command line editor. Input to the editor requires two arguments. Where:\n" 
                               "The first argument corresponds to the name of a valid partition and the second to the name of the default folder.\n"; 

int main(int argc, char** argv)
 {

 if(!--argc) {printf(ANNOUNCE); return EXIT_SUCCESS;}
 
 IMS::Store store(PARTITION); 
  
 CMS::Camera camera(store, FOLDER);
 
 CMS::Editor::Shell shell(camera, FOLDER);
  
 shell.start();

 return EXIT_SUCCESS;
 }


