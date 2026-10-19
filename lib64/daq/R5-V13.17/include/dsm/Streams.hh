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
 
#ifndef DSM_STREAMS
#define DSM_STREAMS

namespace DSM {

class Streams { 
public:
  enum Id {XDS=1, GDS=2, LAM=3, GRL=4, INVALID=5};
public:
  static bool valid(Id id) {return id != Id::INVALID;} 
public:
  Streams() {}
public:  
  Streams(const Streams&) = delete;
public:
 ~Streams() {}
};

}

#endif
