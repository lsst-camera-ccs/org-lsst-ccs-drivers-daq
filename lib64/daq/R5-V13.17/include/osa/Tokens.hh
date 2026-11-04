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
**	000 - , 2007
**
**  Revision History:
**	None.
**
** --
*/
 
#ifndef OSA_TOKENS
#define OSA_TOKENS

namespace OSA {

class Tokens {
public:
  enum {MAX_TOKENS=16, MAX_STRING=127};
public: 
  Tokens() : _numof(0) {}
  Tokens(const char* string, char seperator=' ');
public:
  Tokens(const Tokens&);                   
public:  
  Tokens& operator=(const Tokens&); 
public:
 ~Tokens() {} 
public:
  unsigned     numof()  const {return _numof;}      
  const char** vector()       {return _vector;} 
private:
 unsigned     _numof;
 const char* _vector[MAX_TOKENS];
 char        _string[MAX_STRING + 1];
};

}

#endif
