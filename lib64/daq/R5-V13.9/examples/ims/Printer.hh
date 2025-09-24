
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
 
#ifndef IMS_EDITOR_PRINTER
#define IMS_EDITOR_PRINTER

#include <stdint.h>

#include "ims/Id.hh"
#include "ims/Store.hh"
#include "ims/Processor.hh"

namespace IMS {namespace Editor {

class Printer : public IMS::Processor {
public:
  Printer()               = delete; 
  Printer(const Printer&) = delete;  
public:
  Printer(Store&);
public:
 ~Printer() {} 
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

