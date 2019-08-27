
/*
**  Package:
**	
**
**  Abstract:
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
 
#ifndef OSA_EDITOR
#define OSA_EDITOR

#include <stdio.h>

#include "osa/Command.hh"
#include "osa/Qualifier.hh"
#include "osa/Catchall.hh"
#include "osa/Quit.hh"

#include "osa/Table.hh"

namespace OSA {

class Catchall;

class Editor {
public:
  Editor(Quit*);
  Editor(Quit*, int default_qualifier);
public:
 virtual ~Editor() {}
public:
  void start();
public:
  virtual void announce() {} 
public:
  int insert(Command*   command)   {return _command.insert(command);}
  int insert(Qualifier* qualifier) {return _qualifier.insert(qualifier);}
public: 
  bool query(const char* prompt);
public:
  void dump_commands(  int indent=0);
  void dump_qualifiers(int indent=0);
public:
  int      qualifier(char* command);  
  Command* command(const char* name, int qualifier);     
private: 
  Command*   _removeCommand(const char* name)   {return   (Command*)_command.remove(Command::key(name));}
  Qualifier* _removeQualifier(const char* name) {return (Qualifier*)_qualifier.remove(Qualifier::key(name));}  
private:
  Table    _command;
  Table    _qualifier;
  Catchall _catchall;
  int      _default;
  }; 
                 
}

#endif

