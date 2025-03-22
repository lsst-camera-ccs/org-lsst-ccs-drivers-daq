/*
** ++
**  Package: SATA
**	
**
**  Abstract: Storage Manager Plugin Driver
**
**  Implementation of LSST Storage Manager Drive
**
**  Author:
**      M. Huffer (mehsys@slac.stanford.edu) derived from original from 
**      S. Maldonado, SLAC (smaldona@slac.stanford.edu)
**
**  Creation Date:
**	000 - July 20, 2018
**  001 - June 1, 2020
**
**  Revision History:
**	None.
**
** --
*/

#ifndef SATA_DEVICE
#define SATA_DEVICE

#define __need_getopt_newlib

#include <getopt.h>
#include <stdint.h>    

namespace SATA {

class Features; // Forward reference...

class Device {
public:
  enum {SIZEOF_SECTOR   = 512};
  enum {SECTORS_PER_FIS = 16};
  enum {SIZEOF_FIS      = SIZEOF_SECTOR * SECTORS_PER_FIS};
public:
  enum  Feature {
        NOP                 =  0 + 14,   // Word 82
        READ_BUFFER         =  0 + 13,  
        WRITE_BUFFER        =  0 + 12,  
        READ_LOOKAHEAD      =  0 + 6, 
        VOLATILE_WRITECACHE =  0 + 5, 
        POWER_FEATURES      =  0 + 3, 
        SECURITY_FEATURES   =  0 + 1, 
        SMART_FEATURES      =  0 + 0, 
  
        MB1_30                = 16 + 14,  // Word 83
        FLUSH_CACHE_EXT       = 16 + 13, 
        FLUSH_CACHE           = 16 + 12, 
        ADDRESSING48_FEATURES = 16 + 10, 
        SET_FEATURE_ON_SPINUP = 16 + 6,  
        PUIS_FEATURES         = 16 + 5,  
        APM_FEATURES          = 16 + 3, 
        DOWNLOAD_MICROCODE    = 16 + 0, 
  
        MB1_46              = 32 + 14,   // Word 84
        IDLE_WITH_UNLOAD    = 32 + 13, 
        WWW_FEATURES        = 32 + 8, 
        WRITE_DMA_FUA_EXT   = 32 + 6, 
        GPL_FEATURES        = 32 + 5, 
        STREAMING_FEATURES  = 32 + 4, 
        SMART_BIST          = 32 + 3, 
        SMART_ERROR_LOGGING = 32 + 1, 
    
        MB1_62               = 48 + 14,  // Word 119
        DSN_FEATURES         = 48 + 9, 
        ACCESS_ADDRESSABLE   = 48 + 8, 
        EPC_FEATURES         = 48 + 7, 
        SENSE_DATA_REPORTING = 48 + 6, 
        FREEFALL_FEATURES    = 48 + 5, 
        READ_MICROCODE_MODE3 = 48 + 4, 
        READ_LOG_DMA_EXT     = 48 + 3, 
        WRITE_UNCORRECTABLE  = 48 + 2, 
        READ_WRITE_VERIFY    = 48 + 1 
        };
public: 
  static uint32_t offset(uint32_t bytes)  {return bytes /   SIZEOF_SECTOR;}  
  static uint32_t sectors(uint32_t bytes) {return (bytes + (SIZEOF_SECTOR - 1)) / SIZEOF_SECTOR;} 
  static uint32_t bytes(uint32_t sectors) {return sectors * SIZEOF_SECTOR;}           
public:
  Device(const Features& features);
public:
  Device()              = delete;
  Device(const Device&) = delete;
public:  
 ~Device() {}
public:
  const char* model()            const {return _model;}
  const char* serial_number()    const {return _serial_number;} 
  const char* firmware()         const {return _firmware;} 
  uint64_t    features()         const {return _features;}
  uint64_t    features_enabled() const {return _features_enabled;}
  uint64_t    sectors()          const {return _sectors;}   
public:
  const char* name(enum Feature) const; 
public:
  void dump(int indent=0) const;
private:  
  void _dump(uint64_t features, const char* header, int indent) const;
private:
  uint64_t _sectors;
  uint64_t _features;
  uint64_t _features_enabled;
  char     _model[64];
  char     _serial_number[32];
  char     _firmware[16];
  };
    
}

#endif
