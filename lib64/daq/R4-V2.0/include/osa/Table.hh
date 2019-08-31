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
 
#ifndef OSA_TABLE
#define OSA_TABLE

#include "hash/Table.hh"

namespace OSA {

class Table : public Hash::Table {
public:
  Table(uint32_t stride) : Hash::Table(stride) {}
public:
 ~Table();
};

}

#endif
