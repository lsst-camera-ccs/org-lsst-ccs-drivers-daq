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
 
#ifndef HASH_ITERATE
#define HASH_ITERATE

#include "hash/Table.hh"
#include "hash/List.hh"
#include "hash/Element.hh"

namespace Hash {

class Iterate {
public:
  //Iterate() : _list((List*)0) {}
  Iterate(Table& table);
public:
 ~Iterate() {} 
public:
  Element* element();
private:
  Table&   _table;
  List*    _list;
  Element* _element; 
};

}

#endif
