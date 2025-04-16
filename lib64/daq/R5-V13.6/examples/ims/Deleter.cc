
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

#include "Deleter.hh"

using namespace IMS;

#define INDENT 2

/*
** ++
**
**
** --
*/

Editor::Deleter::Deleter(Store& store) :
 IMS::Processor(), 
 _store(store),
 _total(0)
 {
 }    

/*
** ++
**
**
** --
*/

static const char ERROR[] = "Cannot remove image with ID: %s (error = %s)\n";

void Editor::Deleter::process(const Id& id)   
 {

 Image image(id, _store);
 
 int32_t error = image.error();
 
 if(!error) error = image.remove();
 
 char buffer[32];
 
 if(error) printf(ERROR, id.encode(buffer), IMS::Exception::decode(error));
  
 if(!error) _total++;

 return;
 } 
 
/*
** ++
**
**
** --
*/

static const char BLANK[]   = "";
static const char SUMMARY[] = "%*sTotal deleted images=%-d\n";

void Editor::Deleter::summary() const  
 {
 
 printf(SUMMARY, INDENT, BLANK, _total);
 
 return;
 }
  
