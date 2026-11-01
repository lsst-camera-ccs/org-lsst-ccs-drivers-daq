
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
 
#ifndef IMS_EDITOR_DELETER
#define IMS_EDITOR_DELETER

#include <stdint.h>

#include "ims/Id.hh"
#include "ims/Store.hh"
#include "ims/Processor.hh"

namespace IMS {namespace Editor {

class Deleter : public IMS::Processor {
public:
  Deleter()               = delete; 
  Deleter(const Deleter&) = delete;  
public:
  Deleter(Store&);
public:
 ~Deleter() {} 
public:
  void process(const Id&);
public:
  void summary() const;                               
private:
  Store&   _store;
  unsigned _total;
};
 
}}

#endif

