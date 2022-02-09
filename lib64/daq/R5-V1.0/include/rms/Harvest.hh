 
/*
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
**	    000 - April 06, 2011
**
**  Revision History:
**	    None.
**
** --
*/

#ifndef RMS_HARVEST
#define RMS_HARVEST

#include "daq/Location.hh"
#include "rms/InstructionList.hh"

namespace RMS {

class Harvest {
public:
  Harvest()               {}
  Harvest(const Harvest&) {}
public:
  virtual ~Harvest() {} 
public:
  virtual void process(const DAQ::Location&, const InstructionList&, int32_t error) = 0;
};
  
}

#endif

