
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
 
#ifndef CMS_CAMERA
#define CMS_CAMERA

#include "net/ipv4/Address.hh"
#include "osa/TimeStamp.hh"
#include "dvi/Version.hh"
#include "daq/Sensor.hh"
#include "daq/Location.hh"
#include "daq/LocationSet.hh"
#include "dsm/Exception.hh"
#include "dcs/Id.hh"
#include "ims/Store.hh"
#include "ims/ImageMetadata.hh"
#include "rms/InstructionList.hh"
#include "rds/Client.hh"
#include "scs/Client.hh"
#include "emu/Client.hh"

namespace CMS {

class Camera {
public:  
  Camera()              = delete;
  Camera(const Camera&) = delete;
public:  
  Camera(IMS::Store&);
public:
 ~Camera() {}  
public:
  const DSI::Set& sources() const {return _sources;} 
public:
  int32_t trigger(IMS::ImageMetadata&);
public:
  bool sequence(int32_t opcode, OSA::TimeStamp&);  
public:
  RMS::InstructionList science;
  RMS::InstructionList guiding;
  RMS::InstructionList wavefront;
public:
  IMS::Store& store() {return _store;}
private:
  IMS::Store&      _store;
  RDS::Client      _camera;
  SCS::Client      _sequencer;
  EMU::Client      _emulation;
  DAQ::LocationSet _sources;
 };

}

#endif

