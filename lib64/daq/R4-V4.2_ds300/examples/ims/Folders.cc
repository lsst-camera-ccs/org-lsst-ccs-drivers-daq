
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

#include "Folders.hh"

#define INDENT 2

using namespace IMS;

static const char BLANK[] = "";

/*
** ++
**
**
** --
*/

static const char TITLE[] = "The catalog contains the following folders...\n";

Editor::Folders::Folders(Catalog& catalog) :
 IMS::Folders(catalog), 
 _total(0)
 {
 
 printf(TITLE);
 
 }    

/*
** ++
**
**
** --
*/

static const char NAME[] = "%*s%s\n";

void Editor::Folders::process(const char* name)   
 {

 printf(NAME, INDENT, BLANK, name);
 
 _total++;

 return;
 } 
 
/*
** ++
**
**
** --
*/

static const char SUMMARY[] = "Total=%-d\n";

void Editor::Folders::summary() const  
 {
 
 printf(SUMMARY,  _total);
 
 return;
 }
  
