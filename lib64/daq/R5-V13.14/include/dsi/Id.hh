
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
 
#ifndef DSI_ID
#define DSI_ID

#include <stdint.h>

namespace DSI {

class  Id {
public:
  enum {MAX=32, ERROR=33}; // Maximum number of IDs, maximum ID is one less..
public:  
  Id()          = delete;  
  Id(const Id&) = delete; 
public:
 ~Id() {}        
 };

}

#endif

