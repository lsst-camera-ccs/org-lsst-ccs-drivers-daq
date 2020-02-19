#include <stdio.h>

#include "Quit.hh"

static const char HELP[] = 

" \n The quit command causes the editor to exit. It requires no arguments. Before exiting,\n" 
" if the user has issued any commands requiring a reboot of the partitions's RCEs to take effect\n"
" (that is: either <assign> or <remove> commands) the editor will prompt the user as to whether\n"
" or not to reboot (y or n). If the response to that query is positive (y) the partition will be\n"
" be rebooted before the editor exits.\n\n";

using namespace RMS;

/*
** ++
**
**
** --
*/
                                  
void Utility::Quit::process(int argc, const char** argv, int qualifier)
 {
   
 return;
 }

/*
** ++
**
**
** --
*/

void Utility::Quit::help(int argc, const char** argv, int qualifier)   
 {
 
 printf(HELP);
 
 return;
 }
     
  
