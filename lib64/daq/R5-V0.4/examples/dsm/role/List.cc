/*
** ++
**  Package:
**	
**
**  Abstract:
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

#include "dsm/Roles.hh"
#include "dsm/Services.hh"

#include "List.hh"

using namespace DSM::Editor::Role;

static void _roles();
static void _services(const char* role);

static const char LINE[]    = "  %2u %s\n";
static const char SUMMARY[] = "  Total = %d\n"; 
                                        
/*
** ++
**
**
** --
*/

static const char COMMAND[] = "list";

List::List() : 
 Command(COMMAND)
 {
 }

/*
** ++
**
**
** --
*/

#define ROLE argv[0]

void List::process(int argc, const char** argv, int qualifier)   
 {
 
 if(argc)
  _services(ROLE);
 else
  _roles();
   
 return; 
 }

/*
** ++
**
**
** --
*/
                                  
static const char SERVICES[] = "  ID Service name\n"
                               "  -- ------------\n";
 
void _services(const char* role)   
 {
 
 printf(SERVICES);
 
 DSM::Roles    roles;
 DSM::Services services;
 
 DSM::Roles::Id id        = roles.id(role);
 unsigned       remaining = services.list(id);
 unsigned       total     = 0;
 unsigned       sid       = __builtin_ffs(remaining);
  
 while(remaining)
  {
  
  sid--;
  
  printf(LINE, sid, services.name(DSM::Services::Id(sid)));
 
  total++;
  
  remaining &= ~(1 << sid); 
  
  sid = __builtin_ffs(remaining);
  }
 
 printf(SUMMARY, total);
 
 return; 
 }

/*
** ++
**
**
** --
*/

static const char ROLES[] = "  ID Role name\n"
                            "  -- ---------\n";
                                    
void _roles()   
 {
 
 printf(ROLES);
 
 DSM::Roles roles;
 
 unsigned remaining = roles.all();
 unsigned total     = 0;
 unsigned pid       = __builtin_ffs(remaining);
  
 while(remaining)
  {
  
  pid--;
  
  printf(LINE, pid, roles.name(DSM::Roles::Id(pid)));
 
  total++;
  
  remaining &= ~(1 << pid); 
  
  pid = __builtin_ffs(remaining);
  }
 
 printf(SUMMARY, total);
 
 return; 
 }

/*
** ++
**
**
** --
*/

static const char HELP[] = "\n" 
                           "  This command prints, to standard out the list of permitted roles.\n"
                           "  See the <set> command to assign RCEs to a role.\n" 
                           "\n";

void List::help(int argc, const char** argv, int qualifier)   
 {
 
 printf(HELP);

 return;
 }

