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

#ifndef SATA_SERIAL
#define SATA_SERIAL

#define __need_getopt_newlib

#include <getopt.h>
#include <stdint.h>    

namespace SATA {

class Features; // Forward reference...

class Serial {
public:
 enum  Capability {
        READ_LOG_DMA_EXT          =  0 + 15,   // Word 76
        DEVICE_AUTO_SLUMBER       =  0 + 14,  
        HOST_AUTO_SLUMBER         =  0 + 13,  
        NCQ_PRIORITY              =  0 + 12, 
        UNLOAD_NCQ_OUTSTANDING    =  0 + 11, 
        PHY_EVENT_COUNTERS_LOG    =  0 + 10, 
        POWER_MANAGEMENT_REQUESTS =  0 + 9, 
        NCQ_FEATURES              =  0 + 8, 
        GEN3                      =  0 + 3, 
        GEN2                      =  0 + 2, 
        GEN1                      =  0 + 1, 
        POWER_DISABLE_ENABLED     = 16 + 8,  // Word 77
        DEVSLEEP_REDUCED          = 16 + 7, 
        RECEIVE_SEND_FPDMA_QUEUED = 16 + 6, 
        NCQ_NONDATA               = 16 + 5, 
        NCQ_STREAMING             = 16 + 4,  
        SPEED                     = 16 + 1, 
        };  
  enum  Feature {
        DISABLE_POWER             =  12,  
        REBUILD_ASSIST            =  11,  
        HYBRID_INFORMATION        =  9,  
        DEVICE_SLEEP              =  8, 
        NCQ_AUTOSENSE             =  7, 
        PRESERVE_SETTINGS         =  6, 
        CONTROL_HARDWARE          =  5, 
        DELIVER_INORDER           =  4, 
        INITIATE_POWER_MANAGEMENT =  3, 
        DMA_AUTO_ACTIVATION       =  2, 
        NONZERO_OFFSETS           =  1, 
        };  
public:
  Serial(const Features&);
  Serial(const Serial& clone) : _capabilities(clone._capabilities), _features(clone._features), _features_enabled(clone._features_enabled) {}
public:
  Serial() = delete;
public:  
 ~Serial() {}
public:
  uint32_t capabilities()     const {return _capabilities;}   
  uint16_t features()         const {return _features;}
  uint16_t features_enabled() const {return _features_enabled;}
  uint32_t speed()            const {return _speed;}
public:
  const char* name(enum Capability) const; 
  const char* name(enum Feature)    const; 
public:
  void dump(const char* header, int indent=0) const;
private:
  void _dump(uint16_t features, const char* header, int indent) const;  
  void _dump(uint32_t features, const char* header, int indent) const;
private:
  uint32_t _capabilities;
  uint32_t _speed;
  uint16_t _features;
  uint16_t _features_enabled;
  };
    
}

#endif
