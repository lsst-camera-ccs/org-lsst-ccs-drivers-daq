
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

#include "Reader.hh"

using namespace IMS;

static const char ERROR[] = "Error reading element: %s (%s)\n";

/*
** ++
**
**
** --
*/

#define SAMPLES 8192

#define SCIENCE_SIZE   Science::Data::bytes(SAMPLES)
#define GUIDING_SIZE   Guiding::Data::bytes(SAMPLES)
#define WAVEFRONT_SIZE Wavefront::Data::bytes(SAMPLES)

#define TOTAL (SCIENCE_SIZE + GUIDING_SIZE + WAVEFRONT_SIZE)

Editor::Reader::Reader(Image& image) :
 Decoder(image), 
 _processed(0),
 _buffer(new char[TOTAL]),
 _science(  _buffer + 0,                         SAMPLES),
 _guiding(  _buffer + SCIENCE_SIZE,              SAMPLES),
 _wavefront(_buffer + SCIENCE_SIZE+GUIDING_SIZE, SAMPLES)
 {
 }    

/*
** ++
**
**
** --
*/

void Editor::Reader::process(Science::Source& source, uint64_t size, uint64_t base)   
 {
 
 char buffer[16];
   
 uint64_t remaining = size;
 uint64_t offset    = base;

 while(remaining)
  {
 
  uint64_t length = remaining > SCIENCE_SIZE ? SCIENCE_SIZE : remaining; 
  int32_t  error  = _science.read(source, length, offset);

  if(!error)
   _processed += length;
  else
   printf(ERROR, source.location().encode(buffer), Exception::decode(error));
  
  remaining -= length;
  offset    += length;

  }
  
 return;
 } 

/*
** ++
**
**
** --
*/

void Editor::Reader::process(Guiding::Source& source, uint64_t size, uint64_t base)   
 {
 
 char buffer[16];

 uint64_t remaining = size;
 uint64_t offset    = base;
 
 while(remaining)
  {
 
  uint64_t length = remaining > GUIDING_SIZE ? GUIDING_SIZE : remaining; 
  int32_t  error  = _guiding.read(source, length, offset);
 
  if(!error)
   _processed += length;
  else
   printf(ERROR, source.location().encode(buffer), Exception::decode(error));
  
  remaining -= length;
  offset    += length;
   
  }
  
 return;
 } 

/*
** ++
**
**
** --
*/

void Editor::Reader::process(Wavefront::Source& source, uint64_t size, uint64_t base)   
 {
 
 char buffer[16];

 uint64_t remaining = size;
 uint64_t offset    = base;
 
 while(remaining)
  {
 
  uint64_t length = remaining > WAVEFRONT_SIZE ? WAVEFRONT_SIZE : remaining; 
  int32_t  error  = _wavefront.read(source, length, offset);
 
  if(!error)
   _processed += length;
  else
   printf(ERROR, source.location().encode(buffer), Exception::decode(error));
  
  remaining -= length;
  offset    += length;
   
  }
  
 return;
 }  
 
  
