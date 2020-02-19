
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

#include "Inspector.hh"

using namespace IMS;

static const char ERROR[] = "Error reading element: %s (%s)\n";

#define INDENT 2

/*
** ++
**
**
** --
*/

Editor::Inspector::Inspector(Image& image) :
 Decoder(image), 
 _size(0),
 _total(0)
 {
 
  Source::title(2);

 }    

/*
** ++
**
**
** --
*/

void Editor::Inspector::process(Science::Source& source, uint64_t length, uint64_t base)   
 {
  
 source.synopsis(length, INDENT);
 
 _size += length;
 _total++;

 return;
 } 
 
/*
** ++
**
**
** --
*/

void Editor::Inspector::process(Guiding::Source& source, uint64_t length, uint64_t base)  
 {
  
 source.synopsis(length, INDENT);
 
 _size += length;
 _total++;

 return;
 } 

/*
** ++
**
**
** --
*/

void Editor::Inspector::process(Wavefront::Source& source, uint64_t length, uint64_t base)  
 {
 
 source.synopsis(length, INDENT);
 
 _size += length;
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
static const char SUMMARY[] = "%*sTotal=%-2d %14llu (bytes)\n";

void Editor::Inspector::summary() const  
 {
 
 printf(SUMMARY, INDENT, BLANK, _total, _size);
 
 return;
 }
  
