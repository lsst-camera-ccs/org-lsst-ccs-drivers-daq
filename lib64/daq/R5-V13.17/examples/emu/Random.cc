
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

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "Random.hh"

static const char RANDOM[] = "/dev/urandom";

using namespace EMU;

#define SCIENCE_SIZE   IMS::Science::Data::bytes(SAMPLES)
#define GUIDING_SIZE   IMS::Guiding::Data::bytes(SAMPLES) 
#define WAVEFRONT_SIZE IMS::Wavefront::Data::bytes(SAMPLES)

#define TOTAL (SCIENCE_SIZE + GUIDING_SIZE + WAVEFRONT_SIZE)

/*
** ++
**
**
** --
*/

Random::Random() :
  _buffer(new char[TOTAL]),   
  _science(  _buffer + 0,                         SAMPLES),
  _guiding(  _buffer + SCIENCE_SIZE,              SAMPLES),
  _wavefront(_buffer + SCIENCE_SIZE+GUIDING_SIZE, SAMPLES)
  {
  
  int file = ::open(RANDOM, O_RDONLY);
 
  ::read(file, (void*)_buffer, TOTAL);
 
  ::close(file);
   
  }
  
/*
** ++
**
**
** --
*/ 

Random::~Random()
 {
 
 delete _buffer;
 
 }   

