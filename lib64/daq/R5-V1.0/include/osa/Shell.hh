
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

#ifndef OSA_SHELL
#define OSA_SHELL

#include <stdio.h>

#include "osa/Command.hh"
#include "osa/Qualifier.hh"
#include "osa/Catchall.hh"
#include "osa/Quit.hh"

namespace OSA {

class Table;

class Shell {
public:
  Shell(Quit*);
  Shell(Quit*, int default_qualifier);
public:
 virtual ~Shell();
public:
  void start();
public:
  virtual void announce() {} 
public:
  int insert(Command*   command);
  int insert(Qualifier* qualifier);
public: 
  bool query(const char* prompt);
public:
  void dump_commands(  int indent=0);
  void dump_qualifiers(int indent=0);
public:
  int      qualifier(char* command);  
  Command* command(const char* name, int qualifier);     
private: 
  Command*   _removeCommand(const char* name);
  Qualifier* _removeQualifier(const char* name);
private:
  Table*    _command;
  Table*    _qualifier;
  Catchall  _catchall;
  int       _default;
  }; 
                 
}

#endif

