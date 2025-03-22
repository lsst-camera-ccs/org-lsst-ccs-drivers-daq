#ifndef GDS_CLEARFILE
#define GDS_CLEARFILE

#include <stdio.h>

#include "gds/ClearParameters.hh"

namespace GDS {

class ClearFile
{
public:
  ClearFile(const char* filename);
  ClearFile() = delete;
public:
  ~ClearFile();
public:
  bool fillParams(ClearParameters&);
public:
  void dump();
private:
  char  _next_delim();
  char* _get_key(char*);
  int   _get_value();
private:
  FILE* _file;
};
}

#endif
