#include <zlib.h>
#include <stdio.h>

#include "Crc32.hh"


using namespace IMS;

//OK
//#define PAGE_SIZE 42135552 // 195072*216
#define PAGE_SIZE 21067776 // 195072*108
//#define PAGE_SIZE 1755648    // LCM of One Page and One Stripe (108B)
//#define PAGE_SIZE 975360   // 195072*5
//#define PAGE_SIZE 780288   // 195072*4
//#define PAGE_SIZE 585216   // 195072*3
//#define PAGE_SIZE 390144   // 195072*2
//#define PAGE_SIZE 195072   // One Page
//#define PAGE_SIZE 65024    // 195072/3 // One Pagelet
//#define PAGE_SIZE 32512    // 195072/6
//#define PAGE_SIZE 16256    // 195072/12
//#define PAGE_SIZE 8128     // 195072/24
//#define PAGE_SIZE 4064     // 195072/48
//#define PAGE_SIZE 2032     // 195072/96
//#define PAGE_SIZE 1016     // 195072/192

/*
** ++
**
**
** --
*/

Editor::Crc32::Crc32(IMS::Image& image) : 
  IMS::Decoder(image),
  _locs(),
  _buffer(new uint8_t[PAGE_SIZE]),
  _crc{},
  _size{}
{
  //unsigned remaining = DAQ::LocationSet::SIZE;
  //while(remaining) {_crc[remaining] = crc32(0, NULL, 0); _size[remaining--] = 0;}
};

/*
** ++
**
**
** --
*/

Editor::Crc32::Crc32(IMS::Image& image, const DAQ::LocationSet& filter) : 
  IMS::Decoder(image, filter),
  _locs(),
  _buffer(new uint8_t[PAGE_SIZE]),
  _crc{},
  _size{}
{
  //unsigned remaining = DAQ::LocationSet::SIZE;
  //while(remaining) {_crc[remaining] = crc32(0, NULL, 0); _size[remaining--] = 0;}
};

/*
** ++
**
**
** --
*/

Editor::Crc32::~Crc32() 
{
  delete[] _buffer;
}

/*
** ++
**
**
** --
*/

void Editor::Crc32::process(IMS::Science::Source&   source, uint64_t length, uint64_t offset) 
{
  _calc(source, length, offset);

  if(source.counters().read.retries || source.counters().read.timouts)
    source.counters().read.dump();
}

/*
** ++
**
**
** --
*/

void Editor::Crc32::process(IMS::Guiding::Source&   source, uint64_t length, uint64_t offset) 
{
  _calc(source, length, offset);

  if(source.counters().read.retries || source.counters().read.timouts)
    source.counters().read.dump();
}

/*
** ++
**
**
** --
*/

void Editor::Crc32::process(IMS::Wavefront::Source& source, uint64_t length, uint64_t offset) 
{
  _calc(source, length, offset);

  if(source.counters().read.retries || source.counters().read.timouts)
    source.counters().read.dump();
}

/*
** ++
**
**
** --
*/

void Editor::Crc32::_calc(IMS::Source& source, uint64_t length, uint64_t base) 
{
  _locs.insert(source.location());
  
  uint64_t remaining = length;
  while(remaining)
  {
    uint64_t size = PAGE_SIZE<remaining ? PAGE_SIZE : remaining;
    source.read((char*)_buffer, size);
    _crc[source.location().index()] = crc32(_crc[source.location().index()], _buffer, size);
    _size[source.location().index()] += size;
    if(source.counters().read.retries || source.counters().read.timouts)
      source.counters().read.dump();
    remaining -= size;
  }
}

/*
** ++
**
**
** --
*/

uint32_t Editor::Crc32::crc() const
{
  uint32_t crc = 0;
  DAQ::LocationSet remaining = _locs;
  DAQ::Location loc;
  while(remaining.remove(loc))
    crc = crc32_combine(crc, _crc[loc.index()], _size[loc.index()]);

  return crc;
}

/*
** ++
**
**
** --
*/

uint64_t Editor::Crc32::size() const
{
  uint64_t s = 0;
  DAQ::LocationSet remaining = _locs;
  DAQ::Location loc;
  while(remaining.remove(loc)) s += size(loc);

  return s;
}

/*
** ++
**
**
** --
*/

static const char  SUMMARY[] = "%*s%s - %016llX - ALL %12llu - 0x%08X\n";
static const char* SPACE = "";

void Editor::Crc32::dump(int indent) const
{
  DAQ::LocationSet locs = _locs;
  bool summary = (locs.numof()>1);
  DAQ::Location loc;
  while(locs.remove(loc)) dump(loc, indent);
  if(summary) dump(DAQ::Location::UNDEFINED, indent);
}

/*
** ++
**
**
** --
*/

static const char  CRC[] = "%*s%s - %016llX - %4s - %9llu - 0x%08X\n";

void Editor::Crc32::dump(const DAQ::Location& loc, int indent) const
{

  if(DAQ::Location::UNDEFINED == loc.index())
  {
    printf(SUMMARY, indent, SPACE, _image.metadata().timestamp().decode(),
	   (unsigned long long)_image.id().value(),
	   (unsigned long long)size(),
	   (unsigned)crc());
  }
  else
  {
    char locbuf[8];
    if(!size(loc)) return;
    printf(CRC, indent, SPACE, _image.metadata().timestamp().decode(),
	   (unsigned long long)_image.id().value(), loc.encode(locbuf),
	   (long long unsigned)size(loc), crc(loc));
  }
}
