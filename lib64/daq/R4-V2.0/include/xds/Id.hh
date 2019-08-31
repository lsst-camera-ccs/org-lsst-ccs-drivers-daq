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
 
#ifndef XDS_ID
#define XDS_ID

#include "dcs/Id.hh"

namespace XDS {

class __attribute__((__packed__)) Id : public DCS::Id {
public:
  enum Space : uint8_t {XDS=0, IMS=1};
public:
  Id() = delete;
public:  
  Id(bool);
  Id(                Space space, uint8_t type, uint8_t instance);
  Id(const char* id, Space space, uint8_t type, uint8_t instance);
  Id(uint64_t id);
  Id(const DCS::Id&, Space space, uint8_t type, uint8_t instance);
  Id(const Id&);
public:
 ~Id() {}
public:  
  Id& operator=(const Id&);
public:
  Space   space()    const {return _space;}
  uint8_t type()     const {return _type;}
  uint8_t instance() const {return _instance;}  
public:
  void dump(int indent=0) const;   
private:
  Space   _space;
  uint8_t _type; 
  uint8_t _instance;
  uint8_t _reserved;   
};

}
 
#endif

