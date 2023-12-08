#ifndef GDS_ROIFILE
#define GDS_ROIFILE

#include <stdio.h>

#include "gds/RoiCommon.hh"
#include "gds/RoiLocation.hh"

namespace GDS {

class RoiFile
{
public:
  RoiFile(const char* filename);
  RoiFile() = delete;
public:
  ~RoiFile();
public:
  bool islist() const {return _list;}
public:
  bool next(RoiCommon&, RoiLocation locations[], unsigned& nlocs);
public:
  int series() const {return _series;}
public:
  void dump();
private:
  char  _next_delim();
  char* _get_key(char*);
  bool  _fill_common(GDS::RoiCommon&);
  int   _get_value();
  bool  _fill_location(const GDS::Location&, GDS::RoiLocation&);
private:
  FILE* _file;
  bool _list;
  int  _series;
};
}

#endif
