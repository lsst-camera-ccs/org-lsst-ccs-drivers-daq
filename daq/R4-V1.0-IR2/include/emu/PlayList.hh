
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
 
#ifndef EMU_PLAYLIST
#define EMU_PLAYLIST

#include "osa/Bucket.hh"
#include "ims/Id.hh"

namespace EMU {

class PlayList {
public:
  enum {MAX=8176};
public:  
  PlayList();     
  PlayList(const char* backing_file);
public:       
  PlayList(const PlayList&) = delete;                         
public:
 ~PlayList() {}
public:
  unsigned       length() const;
  unsigned       size()   const;
  const IMS::Id* vector() const;
public:
  bool insert(const IMS::Id&);
private:
  friend class player;  
private:
  OSA::Bucket _playlist;
  unsigned    _offset;
  unsigned    _remaining;
  };

}

#endif

