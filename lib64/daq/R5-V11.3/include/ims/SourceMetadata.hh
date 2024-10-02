
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
 
#ifndef IMS_SOURCEMETADATA
#define IMS_SOURCEMETADATA

#include <stdint.h>

#include "dvi/Version.hh"
#include "osa/TimeStamp.hh"
#include "daq/Location.hh"
#include "daq/Sensor.hh"
#include "daq/Path.hh"
#include "rms/InstructionList.hh"

namespace IMS {

class __attribute__((__packed__)) SourceMetadata  {
public:  
  static void title(int indent=0);
public:
  SourceMetadata(); 
  SourceMetadata(DAQ::Sensor::Type, const OSA::TimeStamp&); 
  SourceMetadata(DAQ::Sensor::Type, DAQ::Path::LANE, const char* platform);
  SourceMetadata(const SourceMetadata&);
public:
 ~SourceMetadata() {}
public:
  SourceMetadata& operator=(const SourceMetadata&);
  SourceMetadata& operator=(const RMS::InstructionList&);
  SourceMetadata& operator=(uint64_t serial_number);
  SourceMetadata& operator=(uint32_t firmware);
  SourceMetadata& operator=(const OSA::TimeStamp&);
public:
  DAQ::Sensor::Type           sensor()        const;
  DAQ::Path::LANE             lane()          const;
  const char*                 platform()      const;
  const DVI::Version&         software()      const;
  uint32_t                    firmware()      const;
  uint64_t                    serial_number() const;  
  const OSA::TimeStamp&       timestamp()     const;
  const RMS::InstructionList& instructions()  const;
public:
  void dump(const DAQ::Location& location, uint64_t length, int indent=0) const;
private:
  uint8_t              _schema;     
  DAQ::Path            _path;
  DAQ::Sensor::Type    _sensor;
  uint8_t              _reserved; 
  uint32_t             _firmware;  
  DVI::Version         _software;
  char                 _platform[32]; 
  uint64_t             _serial_number;
  RMS::InstructionList _instructions;
  OSA::TimeStamp       _timestamp;
  
};

}

#endif

