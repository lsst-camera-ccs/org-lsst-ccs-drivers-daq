
#ifndef GDS_SUBSCRIBER
#define GDS_SUBSCRIBER

#include "net/ipv4/Buffer.hh"
#include "net/ipv4/Datagram.hh"

#include "gds/LocationSet.hh"
#include "gds/SeriesMetadata.hh"
#include "gds/StateMetadata.hh"
#include "gds/RawStamp.hh"

namespace GDS {

namespace Stream{class Pool; class Address; class Socket;}

class Subscriber : public IPV4::Buffer
{
public:
  Subscriber(const char* partition, const LocationSet&);
public:
  virtual ~Subscriber();
public:
  virtual void wait();
public:
  void abort() {_abort = true;}
  operator bool() {return !_abort;}
protected:
  virtual void start     (const StateMetadata&, const SeriesMetadata&) {};
  virtual void resume    (const StateMetadata&) {};
  virtual void pause     (const StateMetadata&) {};
  virtual void stop      (const StateMetadata&) {};
  virtual void raw_stamp (const StateMetadata&, const RawStamp&) {};
protected:
  bool accepted(IPV4::Datagram*, const StateMetadata&);
public:
  friend class Decoder;
private:
  LocationSet      _accepts;
  Stream::Address* _stream;
  Stream::Socket*  _socket;
  Stream::Pool*    _pool;
private:
  bool  cached() const {return false;}
  void* allocate(size_t size, const uint64_t header[]);
private:
  bool _abort;
};
}
#endif
