
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
#include "net/ipv4/Address.hh"
#include "net/ipv4/Socket.hh"
#include "dsi/Location.hh"
#include "dsi/Exception.hh"
#include "dsi/Frame.hh"
#include "dsi/Socket.hh"

namespace DSI {

class Service : protected Socket {
protected:
  Service(const IPV4::Address&, const Location&, const char* interface) throw (Exception); 
  Service(const IPV4::Address&,                  const char* interface) throw (Exception); 
protected:
  Service()               = delete;
  Service(const Service&) = delete;
public:
  virtual ~Service();
public: 
  enum Frame::Function start() __attribute__((noinline)); 
public:
  void ack(Frame&);
protected:
  virtual void M00(Frame& frame, const void* input, int size) {return;}
  virtual void M01(Frame& frame, const void* input, int size) {return;}
  virtual void M02(Frame& frame, const void* input, int size) {return;}
  virtual void M03(Frame& frame, const void* input, int size) {return;}
  virtual void M04(Frame& frame, const void* input, int size) {return;}
  virtual void M05(Frame& frame, const void* input, int size) {return;}
  virtual void M06(Frame& frame, const void* input, int size) {return;}
  virtual void M07(Frame& frame, const void* input, int size) {return;} 
  virtual void M08(Frame& frame, const void* input, int size) {return;}
  virtual void M09(Frame& frame, const void* input, int size) {return;}
  virtual void M10(Frame& frame, const void* input, int size) {return;}
  virtual void M11(Frame& frame, const void* input, int size) {return;}
  virtual void M12(Frame& frame, const void* input, int size) {return;}
  virtual void M13(Frame& frame, const void* input, int size) {return;}
  virtual void M14(Frame& frame, const void* input, int size) {return;}
  virtual void M15(Frame& frame, const void* input, int size) {return;} 
  virtual void M16(Frame& frame, const void* input, int size) {return;}
  virtual void M17(Frame& frame, const void* input, int size) {return;}
  virtual void M18(Frame& frame, const void* input, int size) {return;}
  virtual void M19(Frame& frame, const void* input, int size) {return;}
  virtual void M20(Frame& frame, const void* input, int size) {return;}
  virtual void M21(Frame& frame, const void* input, int size) {return;}
  virtual void M22(Frame& frame, const void* input, int size) {return;}
  virtual void M23(Frame& frame, const void* input, int size) {return;} 
  virtual void M24(Frame& frame, const void* input, int size) {return;}
  virtual void M25(Frame& frame, const void* input, int size) {return;}
  virtual void M26(Frame& frame, const void* input, int size) {return;}
  virtual void M27(Frame& frame, const void* input, int size) {return;}
  virtual void M28(Frame& frame, const void* input, int size) {return;}
  virtual void M29(Frame& frame, const void* input, int size) {return;}
private:  
  virtual void lookup(  Frame&, const void* input, int size);          // 28...
  virtual void probe(   Frame&, const void* input, int size);          // 29...
  virtual void restart( Frame&, const void* input, int size);          // 30...
  virtual void shutdown(Frame&, const void* input, int size);          // 31...    
  virtual void catchall(Frame&, const void* input, int size) {return;} // 32     
protected:
  void stop(enum Frame::Function function) {_stop |= 1 << function;}
private:
  typedef void (Service::* Method)(Frame&, const void* input, int size);       
private:
  void _call(unsigned function, Frame& frame, const char* input, int size) {Method method = _lut[function]; (this->*method)(frame, input, size); return;} 
private:
  void _construct_lut();
private:
  DVI::Version  _version;
  char*         _buffer; 
  Method        _lut[Frame::Function::UNDEFINED+1]; // include _catchall...
  unsigned      _stop;
  DSI::Location _location;
  }; 
                 
}

#endif

