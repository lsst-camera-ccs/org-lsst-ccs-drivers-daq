
#ifndef GDS_SUBSCRIBER
#define GDS_SUBSCRIBER

#include "net/ipv4/Buffer.hh"
#include "net/ipv4/Datagram.hh"

#include "gds/LocationSet.hh"
#include "gds/SeriesMetadata.hh"
#include "gds/StateMetadata.hh"
#include "gds/RawStamp.hh"

namespace GDS {

namespace Stream{class Address; class Socket; class RawBuffer;}

class Subscriber : public IPV4::Buffer
{
public:
  Subscriber(const char* partition, const LocationSet&);
public:
  virtual ~Subscriber() {};
public:
  virtual void wait();
public:
  void     abort()       {_abort = true;}
  operator bool()        {return !_abort;}
private:
  virtual void start     (const StateMetadata&, const SeriesMetadata&) {};
  virtual void resume    (const StateMetadata&) {};
  virtual void pause     (const StateMetadata&) {};
  virtual void stop      (const StateMetadata&) {};
  virtual void raw_stamp (const StateMetadata&, const RawStamp&) {};
public:
  unsigned err_xfer(const Location& loc)     const {return _err_xfer[loc.index()];}
  unsigned err_size(const Location& loc)     const {return _err_size[loc.index()];}
  unsigned err_miss_beg(const Location& loc) const {return _err_miss_beg[loc.index()];}
  unsigned err_miss_end(const Location& loc) const {return _err_miss_end[loc.index()];}
private:
  virtual void _stamp(const StateMetadata&, const Stream::RawBuffer&) {};
private:
  void _start    (const StateMetadata&, const IPV4::Datagram&);
  void _stop     (const StateMetadata&, const IPV4::Datagram&);
  void _raw_stamp(const StateMetadata&, const IPV4::Datagram&);
private:
  bool _accepted(IPV4::Datagram*, const StateMetadata&);
private:
  LocationSet        _accepts;
  Stream::Address&   _stream;
  Stream::Socket&    _socket;
  Stream::RawBuffer* _pool[GDS::Set::SIZE];
private:
  bool  cached() const {return false;}
  void* allocate(size_t size, const uint64_t header[]);
private:
  bool _abort;
  unsigned _err_xfer[GDS::Set::SIZE];
  unsigned _err_size[GDS::Set::SIZE];
  unsigned _err_miss_beg[GDS::Set::SIZE];
  unsigned _err_miss_end[GDS::Set::SIZE];
};
}
#endif
