
/*
**  Package:
**	
**
**  Abstract:
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
 
#ifndef IMS_STATSUBSCRIBER
#define IMS_STATSUBSCRIBER

#include "daq/LocationSet.hh"
#include "ims/Subscriber.hh"
#include "ims/Store.hh"
#include "ims/Exception.hh"
#include "ims/Image.hh"
#include "ims/Source.hh"
#include "ims/Slice.hh"

namespace IMS {

class StatSubscriber : public Subscriber {
public:
  StatSubscriber(const Store&, const DAQ::LocationSet&, const char* group) throw (Exception); 
public:  
  virtual ~StatSubscriber() {}
public:
  void process(                             const Image&);
  void process(const Source&,               const Image&);
  void process(const Slice&, const Source&, const Image&);    
public:
  uint64_t images;
  uint64_t sources;
  uint64_t slices;
  uint64_t stripes;
  }; 

}

#endif

