
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

#include "Decoder.hh"

using namespace IMS;

static const char ERROR[] = "Error decoding element: %s (%s)/n";

/*
** ++
**
**
** --
*/

#define SAMPLES 8192

Editor::Decoder::Decoder(Image& image) :
 IMS::Decoder(image), 
 _size(0),
 _science(SAMPLES),
 _guiding(SAMPLES),
 _wavefront(SAMPLES)
 {
 }    

/*
** ++
**
**
** --
*/

#define SCIENCE_SIZE Science::Data::bytes(SAMPLES)

void Editor::Decoder::process(Science::Source& source, uint64_t size, uint64_t base)   
 {
 
 char buffer[8];

 uint64_t remaining = size;
 
 while(remaining)
  {
  
  uint64_t length = remaining > SCIENCE_SIZE ? SCIENCE_SIZE : remaining; 
  int32_t  error  = _science.read(source, length);
    
  if(!error)
   _size += length; 
  else
   printf(ERROR, source.location().encode(buffer), Exception::decode(error));
 
 remaining -= length;
 
 }
  
 return;
 } 
  
/*
** ++
**
**
** --
*/

#define GUIDING_SIZE Guiding::Data::bytes(SAMPLES)

void Editor::Decoder::process(Guiding::Source& source, uint64_t size, uint64_t base)  
 {
 
 char buffer[8];

 uint64_t remaining = size;
 
 while(remaining)
  {
  
  uint64_t length = remaining > GUIDING_SIZE ? GUIDING_SIZE : remaining; 
  int32_t  error  = _guiding.read(source, length);
    
  if(!error)
   _size += length; 
  else
    printf(ERROR, source.location().encode(buffer), Exception::decode(error));
 
 remaining -= length;
 
 }
     
 return;
 } 
 
/*
** ++
**
**
** --
*/

#define WAVEFRONT_SIZE Wavefront::Data::bytes(SAMPLES)

void Editor::Decoder::process(Wavefront::Source& source, uint64_t size, uint64_t base)  
 {

 char buffer[8];

 uint64_t remaining = size;
 
 while(remaining)
  {
  
  uint64_t length = remaining > WAVEFRONT_SIZE ? WAVEFRONT_SIZE : remaining;   
  int32_t  error  = _wavefront.read(source, length);
    
  if(!error)
   _size += length; 
  else
    printf(ERROR, source.location().encode(buffer), Exception::decode(error));
 
 remaining -= length;
 
 }
   
 return;
 } 

 
  
