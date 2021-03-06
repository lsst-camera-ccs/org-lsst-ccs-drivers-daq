
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
 
#ifndef IMS_GUIDING_SOURCE
#define IMS_GUIDING_SOURCE

#include "ims/Source.hh"
#include "ims/Id.hh"
#include "ims/Store.hh"
#include "ims/SourceMetadata.hh"

namespace IMS { namespace Guiding {

class Source : public IMS::Source {
public:
  Source(const Source& source)                                                                                    : IMS::Source(source)                       {}
  Source(const IMS::Id& id, const DAQ::Location& element, IMS::Store& store)                                      : IMS::Source(id, element, store)           {}
  Source(const IMS::Id& id, const DAQ::Location& element, IMS::Store& store, const IMS::SourceMetadata& metadata) : IMS::Source(id, element, store, metadata) {}
public:
  Source() = delete;
public:
 ~Source() {}
public:  
  Source& operator=(const Source& clone) {IMS::Source::operator=(clone); return *this;}
};

}}

#endif

