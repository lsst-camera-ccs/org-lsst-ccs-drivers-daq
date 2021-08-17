
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
 
#ifndef RDS_EVENT
#define RDS_EVENT

#include "osa/TimeStamp.hh"
#include "daq/Sensor.hh"
#include "ims/Id.hh"
#include "rms/InstructionList.hh"

namespace RDS {

class __attribute__((__packed__)) Event {
public:
  Event()             = delete;
  Event(const Event&) = delete;
public:  
  Event(const OSA::TimeStamp&);
  Event(const OSA::TimeStamp&, const RMS::InstructionList& science, const RMS::InstructionList& guiding, const RMS::InstructionList& wavefront);
public:
 ~Event() {}
public:
  const IMS::Id& id()               const {return _id;}
  const OSA::TimeStamp& timestamp() const {return _timestamp;}
public:  
  const RMS::InstructionList& science()   const {return _science;}
  const RMS::InstructionList& guiding()   const {return _guiding;} 
  const RMS::InstructionList& wavefront() const {return _wavefront;} 
public:
  const RMS::InstructionList* instructions(DAQ::Sensor::Type type) const {const RMS::InstructionList* instructions = &_science; return &instructions[(int)type-1];}
private:
  IMS::Id        _id;
  OSA::TimeStamp _timestamp;
private:
  RMS::InstructionList _science; 
  RMS::InstructionList _guiding; 
  RMS::InstructionList _wavefront; 
  };
}

#endif

