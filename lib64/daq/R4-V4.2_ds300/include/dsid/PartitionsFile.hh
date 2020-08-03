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
 
#ifndef DSID_PARTITIONSFILE
#define DSID_PARTITIONSFILE

#include "osa/Bucket.hh"
#include "osa/Path.hh"
#include "dsi/Id.hh" 
#include "dsi/LocationSet.hh"
#include "dsid/Symbol.hh"

namespace DSID {

class PartitionsFile {
public:
  PartitionsFile(const char* directory);
public: 
  PartitionsFile()                = delete;
  PartitionsFile(PartitionsFile&) = delete;
public:
 ~PartitionsFile() {}
public:
  Symbol* symbols() {return (Symbol*)_file.buffer(0, sizeof(Symbol[DSI::Id::MAX]));}
private:
  OSA::Path   _path;
  OSA::Bucket _file;
 };

}

#endif
