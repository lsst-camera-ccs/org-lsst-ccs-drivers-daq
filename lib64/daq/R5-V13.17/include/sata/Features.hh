/*
** ++
**  Package: SATA
**	
**  SATA IDENTIFY_DEVICE_DATA structure

** Follow convention that what SATA convention calls a "Logical Sector is called here a "Block"
**
**  Author:
**      M. Huffer (mehsys@slac.stanford.edu)
**      Extracted from Microsoft Docs, Hardware Dev Center  
**      Link: "Docs/Windows/Programming reference for Windows Driver Kit/Storage/Ata.h"
**
**  Creation Date:
**	000 - July 20, 2020
**
**  Revision History:
**	None.
**
** --
*/

#ifndef SATA_FEATURES
#define SATA_FEATURES

#define __need_getopt_newlib
#include <getopt.h>
#include <stdint.h>   

namespace SATA {

class  __attribute__((aligned (32))) __attribute__((packed)) Features {
public:
  Features() {}
public:
  Features(const Features&) = delete;
public:  
 ~Features() {}
private:
  friend class Device;
private:
  enum {SIZEOF_SN = 10, SIZEOF_FIRMWARE = 4, SIZEOF_MODEL = 20}; // Character string sizes (in sixteen bit words)..
public:
  uint16_t _opaque_0[10];
  uint16_t _serial_number[SIZEOF_SN];    // As a character string with *no* NULL termination in reverse order
  uint16_t _retired_20[2];
  uint16_t _obsolete_22[1];
  uint16_t _firmware[SIZEOF_FIRMWARE];   // As a character string with *no* NULL termination in reverse order
  uint16_t _model[SIZEOF_MODEL];         // As a character string with *no* NULL termination in reverse order
  uint16_t _obsolete_47[1];              // Was MaximumBlockTransfer and VendorUnique2
  uint16_t _trusted_computing;
  uint32_t _capabilities;
  uint16_t _obsolete_51[2];
  uint16_t _freefall;
  uint16_t _obsolete_54[5];              // Was NumberOfCurrentCylinders, umberOfCurrentHeads, CurrentSectorsPerTrack, CurrentSectorCapacity, CurrentMultiSectorSetting...
  uint16_t _sanitize;
  uint32_t _addressable_sectors;         // Maximum addressable sectors (28-bit commands)
  uint16_t _obsolete_62[1];
  uint16_t _DMA;
  uint16_t _PIO;
  uint16_t _timing[4];
  uint16_t _additional_support; 
  uint16_t _reserved_70[1+4]; 
  uint16_t _queue; 
  uint16_t _sata_capabilities[2];
  uint16_t _sata_supported;
  uint16_t _sata_enabled;
  uint16_t _device_major_version;
  uint16_t _device_minor_version;
  uint16_t _features1_supported;
  uint16_t _features2_supported;
  uint16_t _features3_supported;
  uint16_t _features1_enabled;
  uint16_t _features2_enabled;
  uint16_t _features3_enabled;
  uint16_t _ultra_DMA;
  uint32_t _erase_time;
  uint16_t _APM;                         // Automatic Power Management
  uint16_t _password;
  uint16_t _reset;                       // MBZ for SATA devices
  uint16_t _obsolete_94[1];              // Was CurrentAcousticValue, RecommendedAcousticValue  
  uint16_t _stream1[3];
  uint32_t _stream2[1];
  uint64_t _sectors;
  uint16_t _stream3[1];
  uint16_t _opaque_105[1];
  uint16_t _block;
  uint16_t _interSeekDelay;
  uint16_t _worldWideName[4];
  uint16_t _reserved_112[4];
  uint16_t _obsolete_116[1];
  uint32_t _block_size;
  uint16_t _features4_supported;
  uint16_t _features4_enabled;
  uint16_t _reserved_121[6];
  uint16_t _obsolete_127;                // Was MsnSupport, reserved_127...
  uint16_t _security;
  uint16_t _vendor_specific_129[31];  
  uint16_t _reserved_160[8]; 
  uint16_t _formfactor;
  uint16_t _opaque_169[1];
  uint16_t _productID[4];
  uint16_t _reserved_174[2];
  uint16_t _mediaSerialNumber[30];
  uint16_t _SCT;
  uint16_t _reserved_207[2];
  uint16_t _blockAlignment;
  uint16_t _readWriteVerifyMode3[2];
  uint16_t _readWriteVerifyMode2[2];
  uint16_t _obsolete_214[3]; // was 2
  uint16_t _mediaRotationRate;
  uint16_t _reserved_218[1];
  uint16_t _obsolete_219[1];  
  uint16_t _readWriteVerifyMode;
  uint16_t _reserved_221[1];
  uint16_t _transport_version_major;
  uint16_t _transport_version_minor;
  uint16_t _reserved_224[6];
  uint64_t _numof_sectorsExtended;  
  uint16_t _minimumMicrocode;
  uint16_t _maximumMicrocode; 
  uint16_t _reserved_236[19];   
  uint16_t _integrity;
  uint8_t  _crc[32]; // Not part of the standard. Contains CRC after transferring page from device to RCE 
  };
  
static_assert(sizeof(Features) == 512+32, "Class SATA::Features is not equal to one sector (512 bytes)");
  
}

#endif
