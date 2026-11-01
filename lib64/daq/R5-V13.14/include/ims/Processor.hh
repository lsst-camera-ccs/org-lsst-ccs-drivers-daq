
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
  
#ifndef IMS_PROCESSOR
#define IMS_PROCESSOR

#include "ims/Id.hh"

namespace IMS {

class Processor {
public:                
  Processor() {}     
  Processor(const Processor&) {}                           
public:
 virtual ~Processor() {}
public:
  virtual void process(const Id&) = 0;
  };

}

#endif


 
 