
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
 
#ifndef IMS_SUBSCRIBER
#define IMS_SUBSCRIBER

#include "daq/LocationSet.hh"
#include "ims/Id.hh"
#include "ims/Store.hh"
#include "ims/Image.hh"
#include "ims/Decoder.hh"
 
namespace IMS {

class Subscriber {
public:
  Subscriber()                  = delete;
  Subscriber(const Subscriber&) = delete;
public:  
  Subscriber(Store&, const Id&, const DAQ::LocationSet&); 
public:  
 ~Subscriber();
public:
  const Image& wait();
public:  
  void run(Decoder&);
private:
  Id               _folder;
  DAQ::LocationSet _accepts;
  Store&           _store;
  }; 

}

#endif

