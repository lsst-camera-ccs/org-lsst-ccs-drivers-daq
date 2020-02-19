
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
 
#ifndef DSS_SUBSCRIBER
#define DSS_SUBSCRIBER

#include "daq/LocationSet.hh"
#include "dsi/Subscriber.hh"
#include "dsi/Set.hh"
#include "dss/Page.hh"
#include "dss/Open.hh"
#include "dss/Close.hh"
 
namespace DSS {

class Subscriber : public DSI::Subscriber {
public:
  Subscriber() = delete;
  Subscriber(const char* partition, const DSI::Set&) throw (DSI::Exception);
  Subscriber(const char* partition)                  throw (DSI::Exception); 
public:  
  virtual ~Subscriber() {}
public:
  void wait() {_block();}
public:  
  virtual bool process(const Open*)  = 0;
  virtual bool process(const Page*)  = 0;     
  virtual bool process(const Close*) = 0;
private:
  bool M08(const void*, int); // Process open publication
  bool M09(const void*, int); // Process commit publication
  bool M11(const void*, int); // Process close publication
  }; 

}

#endif

