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

#include "Dump.hh"

static const char COMMAND[] = "dump";

using namespace EMU;

/*
** ++
**
**
** --
*/

Editor::Dump::Dump(IMS::Store& store, PlayList* playlist) : 
 Command(COMMAND),
 _store(store),
 _playlist(playlist)
 { 
 }

/*
** ++
**
**
** --
*/

#include "ims/Id.hh"
#include "ims/Image.hh"

#define INDENT 2

static const char BLANK[]   = "";
static const char SUMMARY[] = "%*sTotal=%-d\n";

static const char NO_PLAYLIST[]  = "A playlist was not passed as an argument when starting the utility\n"; 
static const char NOSUCH_IMAGE[] = "%*s%016llX ------- Image with this ID not found in catalog ----------\n";
                           
void Editor::Dump::process(int argc, const char** argv, int qualifier)   
 {
 
 PlayList* playlist = _playlist;
 
 if(!playlist) {printf(NO_PLAYLIST); return;}
 
 IMS::Image::title(INDENT);
  
 unsigned remaining = playlist->length();
 const IMS::Id* id  = playlist->vector();
 unsigned total     = 0;

 while(remaining--)
  {
   
  IMS::Image image(*id, _store);
 
  if(image)
   image.synopsis(INDENT);
  else
   printf(NOSUCH_IMAGE, INDENT, BLANK, id->value());
   
  total++;

  id++;
  
  }
 
 printf(SUMMARY, INDENT, BLANK, total);
 
 return; 
 }
      
/*
** ++
**
**
** --
*/

static const char HELP[] = "\n"
                           "  Print to standard out a synoposis of each image (as represented by its ID)\n" 
                           "  within in the playlist\n"
                           "\n";

void Editor::Dump::help(int argc, const char** argv, int qualifier)   
 {
 
 printf(HELP);
 
 return;
 }
    
