
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

#include "Generator.hh"

using namespace EMU;

static const char ERROR[] = "Error encoding element: %s (%s)\n";

/*
** ++
**
**
** --
*/

Generator::Generator(IMS::Image& image) :
 IMS::Encoder(image), 
 _data(),
 _size(0)
 {
 }    

/*
** ++
**
**
** --
*/

void Generator::process(IMS::Science::Source& source)   
 {
  
 int32_t error = _data.science().write(source);
 
 char buffer[16];
 
 if(!error)
   _size += _data.science().size();
 else
   printf(ERROR, source.location().encode(buffer), IMS::Exception::decode(error));
  
 return;
 } 
 
/*
** ++
**
**
** --
*/

void Generator::process(IMS::Guiding::Source& source)  
 {
 
 int32_t error = _data.guiding().write(source);
 
 char buffer[16];
 
 if(!error)
   _size += _data.guiding().size();
 else
   printf(ERROR, source.location().encode(buffer), IMS::Exception::decode(error));
     
 return;
 } 

/*
** ++
**
**
** --
*/

void Generator::process(IMS::Wavefront::Source& source)  
 {
 
 int32_t error = _data.wavefront().write(source);
 
 char buffer[16];
 
 if(!error)
   _size += _data.wavefront().size();
 else
   printf(ERROR, source.location().encode(buffer), IMS::Exception::decode(error));
   
 return;
 } 

 
 
  
