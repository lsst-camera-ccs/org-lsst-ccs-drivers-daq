
/*
**  Package:
**	
**
**  Abstract:
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
 
/*
**  Package:
**	
**
**  Abstract:
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
 
#ifndef DSI_SERVICE
#define DSI_SERVICE

#include "dvi/Version.hh"

#include "net/ipv4/Buffer.hh"
#include "net/ipv4/Datagram.hh"
#include "net/ipv4/Rundown.hh"
#include "net/ipv4/Exception.hh"

#include "dsi/Socket.hh"
#include "dsi/Address.hh"
#include "dsi/Header.hh"
#include "dsi/Set.hh"
#include "dsi/Location.hh"

namespace DSI {

class Service : public Socket {
protected:
  Service(const Address&, const Location& location,  const char* interface);
  Service(const Address&, const Location& location,  const char* interface, IPV4::Buffer*);
protected:
  Service()               = delete;
  Service(const Service&) = delete;
public:
  virtual ~Service();
public:
  const Location& location() const {return _location;}
  const Set&      servers()  const {return _servers;}
public:
  void accept(uint8_t server)         {_servers.insert(server);}
  void accept(const Location& server) {_servers.insert(server.index());}
public:  
  bool post_flush(const Header& header, const IPV4::Address& destination, void* payload, size_t payload_length, IPV4::Rundown* rundown=(IPV4::Rundown*)0) {_flush(payload, payload_length); return post(header, destination, payload, payload_length, rundown);}
  bool send_flush(const Header& header, const IPV4::Address& destination, void* payload, size_t payload_length)                                           {_flush(payload, payload_length); return send(header, destination, payload, payload_length);}
public:
  bool post(const Header&, const IPV4::Address&,                                       IPV4::Rundown* rundown=(IPV4::Rundown*)0); 
  bool post(const Header&, const IPV4::Address&, void* payload, size_t payload_length, IPV4::Rundown* rundown=(IPV4::Rundown*)0);
  bool send(const Header&, const IPV4::Address&, void* payload, size_t payload_length);
public:
  void deallocate(IPV4::Datagram* datagram) {_deallocate(datagram);}
public:
  enum Header::Function start();
protected:
  virtual void M00(IPV4::Datagram* datagram) {deallocate(datagram);}
  virtual void M01(IPV4::Datagram* datagram) {deallocate(datagram);}
  virtual void M02(IPV4::Datagram* datagram) {deallocate(datagram);}
  virtual void M03(IPV4::Datagram* datagram) {deallocate(datagram);}
  virtual void M04(IPV4::Datagram* datagram) {deallocate(datagram);}
  virtual void M05(IPV4::Datagram* datagram) {deallocate(datagram);}
  virtual void M06(IPV4::Datagram* datagram) {deallocate(datagram);}
  virtual void M07(IPV4::Datagram* datagram) {deallocate(datagram);} 
  virtual void M08(IPV4::Datagram* datagram) {deallocate(datagram);}
  virtual void M09(IPV4::Datagram* datagram) {deallocate(datagram);}
  virtual void M10(IPV4::Datagram* datagram) {deallocate(datagram);}
  virtual void M11(IPV4::Datagram* datagram) {deallocate(datagram);}
  virtual void M12(IPV4::Datagram* datagram) {deallocate(datagram);}
  virtual void M13(IPV4::Datagram* datagram) {deallocate(datagram);}
  virtual void M14(IPV4::Datagram* datagram) {deallocate(datagram);}
  virtual void M15(IPV4::Datagram* datagram) {deallocate(datagram);} 
  virtual void M16(IPV4::Datagram* datagram) {deallocate(datagram);}
  virtual void M17(IPV4::Datagram* datagram) {deallocate(datagram);}
  virtual void M18(IPV4::Datagram* datagram) {deallocate(datagram);}
  virtual void M19(IPV4::Datagram* datagram) {deallocate(datagram);}
  virtual void M20(IPV4::Datagram* datagram) {deallocate(datagram);}
  virtual void M21(IPV4::Datagram* datagram) {deallocate(datagram);}
  virtual void M22(IPV4::Datagram* datagram) {deallocate(datagram);}
  virtual void M23(IPV4::Datagram* datagram) {deallocate(datagram);} 
  virtual void M24(IPV4::Datagram* datagram) {deallocate(datagram);}
  virtual void M25(IPV4::Datagram* datagram) {deallocate(datagram);}
  virtual void M26(IPV4::Datagram* datagram) {deallocate(datagram);}
  virtual void M27(IPV4::Datagram* datagram) {deallocate(datagram);}
private:
  void M28(IPV4::Datagram* datagram);                        // Implemented, Lookup feature...
  void M29(IPV4::Datagram* datagram);                        // Implemented, Probe for server set...
  void M30(IPV4::Datagram* datagram) {deallocate(datagram);} // Implemented, Restart (abort waiting)...
  void M31(IPV4::Datagram* datagram) {deallocate(datagram);} // Implemented, Shutdown (abort waiting)...   
private:
  typedef void (Service::* Method)(IPV4::Datagram*);       
private:
  bool _accept(Header&, size_t length, unsigned function);
  bool _call(unsigned function, IPV4::Datagram* datagram) {Method method = _lut[function]; (this->*method)(datagram); return _stop & (1 << function);} 
  void _construct_lut(); 
private:
  Method        _lut[Header::Function::UNDEFINED];
  Set           _servers;
  IPV4::Buffer* _buffer;
  DVI::Version  _version;
  unsigned      _stop;
  Location      _location;
  uint8_t       _pad[3];
  Address       _group;
  }; 
                 
}

#endif

