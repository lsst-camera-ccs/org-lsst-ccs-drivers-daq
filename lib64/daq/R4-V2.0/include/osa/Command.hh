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
 
#ifndef OSA_COMMAND
#define OSA_COMMAND

#include "hash/Element.hh"
#include "hash/Key.hh"

namespace OSA {

class Command : public Hash::Element { 
public:
 enum : uint32_t {SEED=0xABADBABE};

 // enum uint32_t {SEED=0xABADBABE};
 // enum  {SEED=0xABADBABE};
public:
  static uint64_t key(const char* name) {return Hash::Key(name, SEED);}
public:
  Command(); 
  Command(const char* name);
public:
 virtual ~Command() {}
public:
  virtual void process(int argc, const char** argv, int qualifier) = 0; 
  virtual void help(   int argc, const char** argv, int qualifier) = 0;    
public:
  const char* name() const {return _name;} 
public:   
  Command* dump(int indent=0);
  Command* dump(const Command* until, int indent=0); 
public:
  Command* flink() {return (Command*)Hash::Element::flink();}
public:   
  bool quit() const {return _quit;}
private:
  friend class Quit;  
private:
  const char* _name;
  bool        _quit; 
};

}

#endif
