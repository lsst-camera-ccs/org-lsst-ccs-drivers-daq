
#ifndef GDS_STAMP
#define GDS_STAMP

namespace GDS {

class __attribute__ ((__packed__)) Stamp 
{
public:
  Stamp(uint8_t* content, uint32_t size) : _content(content), _size(size) {};
  Stamp() = delete;
  Stamp(const Stamp& clone) : _content(clone._content), _size(clone._size) {};
public:
  ~Stamp() {};
public:
  Stamp& operator=(const Stamp& clone) {_content=clone._content; _size=clone._size; return *this;}
public:
  int32_t* pixels()       {return (int32_t*)_content;}
  unsigned npix()   const {return _size/sizeof(int32_t);}
public:
  const uint8_t* content() const {return _content;}
  const uint32_t size()    const {return _size;}
private:
  uint8_t* _content;
  uint32_t _size;
};
}

#endif
