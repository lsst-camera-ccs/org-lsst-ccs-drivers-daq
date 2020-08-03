
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
 
#ifndef DSID_PARTITIONS
#define DSID_PARTITIONS

#include "dsid/Symbol.hh"
#include "PartitionsFile.hh"

namespace DSID {

class SymbolTable;

class Partitions {
public: 
  Partitions(const char* directory);
public:
  Partitions()                  = delete;
  Partitions(const Partitions&) = delete;
public:
 ~Partitions();
public:
  bool valid(uint8_t pid) const;
public:  
  uint8_t assign(const Symbol&);
  uint8_t lookup(const Symbol&);
  uint8_t remove(const Symbol&);  
  void    encode(Symbol symbols[]);
public:  
  uint32_t assigned() const;
public:   
  const Symbol* name(uint32_t id) const; 
private:
  PartitionsFile _file;  
  SymbolTable*  _symbols;
  };
      
}

#endif

