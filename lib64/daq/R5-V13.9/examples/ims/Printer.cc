
/*
** ++
**  Package:
**	
**
**  Abstract:
**      
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

#include "ims/Exception.hh"
#include "ims/Image.hh"

#include "Printer.hh"

using namespace IMS;

#define INDENT 2

/*
** ++
**
**
** --
*/

Editor::Printer::Printer(Store& store) :
 IMS::Processor(), 
 _store(store),
 _total(0)
 {
 
 Image::title(INDENT);

 }    

/*
** ++
**
**
** --
*/

void Editor::Printer::process(const Id& id)   
 {

 Image image(id, _store);
 
 if(!image) return;
 
 image.synopsis(INDENT);
 
 _total++;

 return;
 } 
 
/*
** ++
**
**
** --
*/

static const char BLANK[]   = "";
static const char SUMMARY[] = "%*sTotal=%-d\n";

void Editor::Printer::summary() const  
 {
 
 printf(SUMMARY, INDENT, BLANK, _total);
 
 return;
 }
  
