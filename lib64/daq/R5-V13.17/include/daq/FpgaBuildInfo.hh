#ifndef REB_FPGA_BUILD_INFO
#define REB_FPGA_BUILD_INFO

#include <stdint.h>

namespace REB {

class FpgaBuildInfo {
public:
  enum {BUILD_STRING_REGS = 64};
  enum {GIT_HASH_REGS     =  5};
public:
  enum DumpFlags : unsigned {
    DUMP_ID      = 1 << 0,
    DUMP_BUILD   = 1 << 1,
    DUMP_HASH    = 1 << 2,
    DUMP_VERSION = 1 << 3,
    DUMP_CLOCK   = 1 << 4,
    DUMP_ALL     = 0x1F
  };
public:
  FpgaBuildInfo();
  FpgaBuildInfo(const uint32_t buildRegs[BUILD_STRING_REGS],
                const uint32_t hashRegs[GIT_HASH_REGS],
                uint32_t       fwVersion);
public:
  const char* buildString() const {return _buildString;}
  const char* gitHash()     const {return _gitHash;}
  uint32_t    fwVersion()   const {return _fwVersion;}
public:
  virtual void dump(const char* prefix, unsigned flags=DUMP_ALL, int indent=-1) const;
public:
  virtual ~FpgaBuildInfo() {}
protected:
  void _decode(const uint32_t buildRegs[BUILD_STRING_REGS],
               const uint32_t hashRegs[GIT_HASH_REGS],
               uint32_t       fwVersion);
protected:
  char     _buildString[BUILD_STRING_REGS * 4 + 1];
  char     _gitHash[GIT_HASH_REGS * 8 + 1];
  uint32_t _fwVersion;
};

}

#endif
