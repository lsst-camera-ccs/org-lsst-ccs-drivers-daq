
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

#include "daq/Location.hh"
#include "ims/Exception.hh"

#include "Comparator.hh"

using namespace IMS;

static const char NOSUCH_SOURCE[] = "Cannot access source @ '%s' (error = %s\n";
static const char NOSUCH_DATA[]   = "Cannot read data from source @ '%s' (error = %s\n";
static const char INVALID_SIZE[]  = "Two sources from from source @ '%s\n";

/*
** ++
**
**
** --
*/

#define SAMPLES 8192

Editor::Comparator::Comparator(Image& image, Id& id, Store& store) :
 Decoder(image),
 _store(store),
 _id(id), 
 _processed(0),
 _matched(0),
 _science_a(SAMPLES),
 _science_b(SAMPLES),
 _guiding_a(SAMPLES),
 _guiding_b(SAMPLES),
 _wavefront_a(SAMPLES),
 _wavefront_b(SAMPLES)
 {
 }    
 
/*
** ++
**
**
** --
*/

#define SCIENCE_SIZE Science::Data::bytes(SAMPLES)
 
void Editor::Comparator::process(Science::Source& sourceA, uint64_t size, uint64_t base)   
 {
 
 char buffer[8];
 
 const DAQ::Location& location = sourceA.location();
 
 Science::Source sourceB(_id, location, _store);

 int32_t error = sourceB.error();
 
 if(error) {printf(NOSUCH_SOURCE, location.encode(buffer), Exception::decode(error)); return;}
 
 uint64_t sizeB = sourceB.size();
 
 if(size != sizeB) {printf(INVALID_SIZE, location.encode(buffer)); return;}
 
 uint64_t remaining = size;

 while(remaining)
  {
  
  uint64_t length = remaining > SCIENCE_SIZE ? SCIENCE_SIZE : remaining; 
  int32_t  error  = _science_a.read(sourceA, length);
 
  if(!error) _science_b.read(sourceB, length);
 
  if(error) printf(NOSUCH_DATA, location.encode(buffer), Exception::decode(error));
 
  _processed += size;
  _matched   += _science_a.compare(location, _science_b, size);
  
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

void Editor::Comparator::process(Guiding::Source& sourceA, uint64_t size, uint64_t base)  
 {
 
 char buffer[8];
 
 const DAQ::Location& location = sourceA.location();
 
 Guiding::Source sourceB(_id, location, _store);

 int32_t error = sourceB.error();
 
 if(error) {printf(NOSUCH_SOURCE, location.encode(buffer), Exception::decode(error)); return;}
 
 uint64_t sizeB = sourceB.size();
 
 if(size != sizeB) {printf(INVALID_SIZE, location.encode(buffer)); return;}

 uint64_t remaining = size;

 while(remaining)
  {
  
  uint64_t length = remaining > GUIDING_SIZE ? GUIDING_SIZE : remaining; 
  int32_t  error  = _guiding_a.read(sourceA, length);
 
  if(!error) _guiding_b.read(sourceB, length);
 
  if(error) printf(NOSUCH_DATA, location.encode(buffer), Exception::decode(error));
 
  _processed += size;
  _matched   += _guiding_a.compare(location, _guiding_b, size);
  
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

void Editor::Comparator::process(Wavefront::Source& sourceA, uint64_t size, uint64_t base)  
 {

 char buffer[8];
 
 const DAQ::Location& location = sourceA.location();
 
 Wavefront::Source sourceB(_id, location, _store);

 int32_t error = sourceB.error();
 
 if(error) {printf(NOSUCH_SOURCE, location.encode(buffer), Exception::decode(error)); return;}
 
 uint64_t sizeB = sourceB.size();
 
 if(size != sizeB) {printf(INVALID_SIZE, location.encode(buffer)); return;}

 uint64_t remaining = size;

 while(remaining)
  {
  
  uint64_t length = remaining > WAVEFRONT_SIZE ? WAVEFRONT_SIZE : remaining; 
  int32_t  error  = _wavefront_a.read(sourceA, length);
 
  if(!error) _wavefront_b.read(sourceB, length);
 
  if(error) printf(NOSUCH_DATA, location.encode(buffer), Exception::decode(error));
 
  _processed += size;
  _matched   += _wavefront_a.compare(location, _wavefront_b, size);  
  
  remaining -= length;
  
  }
  
 return;
 } 

 
  
