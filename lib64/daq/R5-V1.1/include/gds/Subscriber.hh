
#ifndef GDS_SUBSCRIBER
#define GDS_SUBSCRIBER

#include "net/ipv4/Buffer.hh"

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
  void wait();
public:
  virtual void start  (const StateMetadata&, const SeriesMetadata&) {};
  virtual void resume (const StateMetadata&) {};
  virtual void pause  (const StateMetadata&) {};
  virtual void stop   (const StateMetadata&) {};
  virtual void stamp  (const StateMetadata&, const RawStamp&) {};
private:
  bool  cached() const {return false;}
  void* allocate(size_t size, const uint64_t header[]);
private: 
  uint8_t _lookup(const char* partition); 
private:
  LocationSet      _accepts;
  Stream::Address* _stream;
  Stream::Socket*  _socket;
  Stream::Pool*    _pool;
};
}
#endif
