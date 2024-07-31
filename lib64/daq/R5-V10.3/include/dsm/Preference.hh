
/*
**  Package:
**	
**
**  Abstract:
**  
** Beware: This is and must be an exact duplicate of the class by the same name found in INIT.
    
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
 
#ifndef DSM_PREFERENCE
#define DSM_PREFERENCE

namespace DSM {

class  Preference {
public:
  static const char* mountpoint();
  static const char* partition();
  static const char* path(); 
  static const char* releasepoint();
  static const char* role();
  static const char* path(int interface);
  static const char* source(int interface);
  static const char* vendor();
  static const char* sequencer();
  static const char* sensors();
public:
  Preference()                  = delete;
  Preference(const Preference&) = delete;
public:
 ~Preference() {}
 };

}

#endif

