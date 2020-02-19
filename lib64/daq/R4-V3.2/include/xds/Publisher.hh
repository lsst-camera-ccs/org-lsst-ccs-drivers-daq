
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
 
#ifndef DSS_PUBLISHER
#define DSS_PUBLISHER

#include "dsi/Client.hh"
#include "dsi/Group.hh"
#include "dsi/Exception.hh"
#include "dss/Close.hh"
#include "dss/Open.hh"
#include "dss/Page.hh"

namespace DSS {

class Publisher {
public:                         
  Publisher(const char* partition) throw (DSI::Exception);     
public:
 ~Publisher() {} 
public:
  void  publish(const Open&);
  void  publish(const Page&);     
  void  publish(const Close&);
private: 
  DSI::Client _client;
  DSI::Group  _service;
  DSI::Set    _all;       
  };

}

#endif

