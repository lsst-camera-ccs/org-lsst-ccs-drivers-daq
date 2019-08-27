
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

#include <stdio.h>

#include "Data.hh"

using namespace IMS;
  
/*
** ++
**
**
** --
*/

static const char ERROR1[] = "Stripe %d for CCD %d @ location %s did not compare. Dumping first stripe...\n";
static const char ERROR2[] = "Dumping second stripe...\n";

uint64_t Data::_compare(const Stripe A[], const Stripe B[], uint64_t length, const DAQ::Location& location, int ccd)
 {

 const Stripe* a = A;
 const Stripe* b = B;
 
 uint64_t index     = 0;
 uint64_t remaining = length;

 while(remaining)
  {

  --remaining;
   
  if(*a++ != *b++) break;
 
  index++;
 
  } 
 
 if(remaining)
  {
 
  char buffer[8];
 
  printf(ERROR1, index, ccd, location.encode(buffer));

  //A[index].dump(2);
  
  //printf(ERROR2);
   
  //B[index].dump(2);
  
  }

 return index;
 }


  


