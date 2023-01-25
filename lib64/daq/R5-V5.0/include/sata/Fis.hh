/*
** ++
**  Package: SATA
**	
**
**  Abstract: SATA driver
**
**  Implementation of SATA driver.
**
**  Author:
**      Michael Huffer, abstracted from original: S. Maldonado, SLAC (smaldona@slac.stanford.edu)
**
**  Creation Date:
**	000 - Oct 10, 2017
**  001 - July 18, 2020
**
**  Revision History:
**	None.
**
** --
*
*/

#ifndef FIS_H
#define FIS_H

/*

 fis_type - SATA FIS command opcodes
 
 SATA FIS sizes in bytes
 sata_fis_h2d_t             20
 sata_fis_d2h_t             20
 sata_fis_dma_act_t          4  
 sata_fis_data_t             4 
 sata_fis_pio_setup_t       20
 
*/


namespace SATA {namespace FIS {

struct Type { 
 enum Value {REGISTER_H2D     = 0x27, 
             REGISTER_D2H     = 0x34, 
             DMA_ACTIVATE_D2H = 0x39, 
             DATA_BID         = 0x46, 
             PIO_SETUP_D2H    = 0x5F}; 
};

struct Command { 
  enum Code {RESET_DEV         = 0x08, 
             NOP               = 0x00,
             READ_LOG_EXT      = 0x2F, 
             DMA_READ_EXT      = 0x25, 
             DMA_WRITE_EXT     = 0x35,
             DMA_WRITE_FUA_EXT = 0x3D, 
             FPDMA_READ        = 0x60, 
             FPDMA_WRITE       = 0x61, 
             ID_DEV            = 0xEC, 
             SET_FEATURES      = 0xEF,
             SET_STANDBY       = 0xE2,
             SET_IDLE          = 0xE3,
             CHECKPOWER        = 0xE5, 
             SMART             = 0xB0};
};

struct SubCmnd { 
  enum {SET_TRANSFER = 0x03, 
        ENABLE_APM   = 0x05, 
        SET_FEATURE  = 0x10, 
        DISABLE_APM  = 0x85}; 
};

struct Feature {                // SATA FIS features - cmd set doc 7.45.17.1 table 109
  enum {NONZERO_BUF     = 0x1, 
        DMA_AUTO_ACT    = 0x2, 
        DEV_PWR_TRAN    = 0x3, 
        IN_ORD_DELIV    = 0x4,
        ASYNC_NOTIFY    = 0x5, 
        SET_PRES        = 0x6, 
        AUTO_SLUMBER    = 0x7, 
        HW_FEAT_CTL     = 0x8, 
        READ_SMART_DATA = 0xD0,
        READ_SMART_LOG  = 0xD5,
        ENABLE_SMART    = 0xD8,
        DISABLE_SMART   = 0xD9,
        RETURN_SMART    = 0xDA};
};

struct Transfer { // cmd set doc 7.45.8 tab1e 105
    enum {UDMA_6 = 0x46};
};    

/*
** Interesting, but not exhaustive list of ATA logs (as specified by their address)
*/

struct LogAddress { 
  enum {   
    DIRECTORY         = 0x0,   
    SMART_SUMMARY     = 0x3,  
    DEVICE_STATISTICS = 0x4,   
    SATA_EVENTS       = 0x11,  
    LBA_STATUS        = 0x19,  
    ADDR_MAX          = 0xE2}; // MAX LOG ADDR
};

/*
** Interesting, but not exhaustive list of log pages for the device statistics log (as specified by their page number)      
*/
 
struct LogPage {    
  enum {
    SUPPORTED     = 0x0,  // Supported log pages
    GENERAL       = 0x1,  // General statistics Table A.6, A.11
    ERRORS        = 0x4,  // General errors
    TEMPERATURES  = 0x5,  // Temperature in degrees C
    TRANSPORT     = 0x6,  // Transport stats A.5.8, A.16
    SSD           = 0x7}; // Solid state device statistics
};

/* 
**  Register - Host to Device FIS
*/

struct __attribute__((packed)) __H2D {
	uint8_t  pad[4];     // Not part of SATA standard. Instead, FOR PPI transactions pad structure to a quadword boundary...
	uint8_t  type;       // See "Type" constants defined above...
	uint8_t  pm_port_c;
	uint8_t  command;    // See "Command" constants defined above...
	uint8_t  features;
	uint8_t  lba[3];
	uint8_t  device;
	uint8_t  lba_exp[3];
	uint8_t  features_exp;
	uint8_t  sector_count;
	uint8_t  sector_count_exp;
	uint8_t  icc;
	uint8_t  control;
	uint32_t reserved;
};


/*
** Register - Device to Host FIS
*/

struct __attribute__((packed)) __D2H {
	uint8_t pad[4];     // Not part of SATA standard. Instead, FOR PPI transactions pad structure to a quadword boundary...
	uint8_t type;       // See "Type" constants defined above...
	uint8_t pm_port_i;
	uint8_t status;     // busy[7], data_ready[6], deferred_wr_err[4]
	uint8_t error;
	uint8_t lba[3];
	uint8_t device;
	uint8_t lba_exp[3];
	uint8_t res1;
	uint8_t sector_count;
	uint8_t sector_count_exp;
	uint8_t res2[2];
	uint8_t res3[4];
};

/*
** DMA Activate - Device to Host FIS
*/

struct __attribute__((packed)) Activate {
	uint8_t pad[4]; // Not part of SATA standard. Instead FOR PPI transactions pad structure to a quadword boundary...
	uint8_t type;   // See "Type" constants defined above...
	uint8_t res1;
	uint8_t res2;
	uint8_t res3;
};

/* 
** Data - Host to Device or Device to Host FIS
*/

struct __attribute__ ((packed)) Data {
	uint8_t pad[4];  // Not part of SATA standard. Instead FOR PPI transactions pad structure to a quadword boundary...
	uint8_t type;    // See "Type" constants defined above...
	uint8_t res1;
	uint8_t res2;
	uint8_t res3;
    uint8_t data[]; 
};

/* 
** PIO Setup - Host to Device or Device to Host FIS
*/

struct __attribute__((packed)) Setup {
	uint8_t pad[4];     // Not part of SATA standard. Instead FOR PPI transactions pad structure to a quadword boundary...
	uint8_t type;       // See "Type" constants defined above...
	uint8_t pm_port_i;
	uint8_t status;     // busy[7], data_ready[6], deferred_wr_err[4]
	uint8_t error;
	uint8_t lba[3];
	uint8_t device;
	uint8_t lba_exp[3];
	uint8_t res1;
	uint8_t sector_count;
	uint8_t sector_count_exp;
	uint8_t res2;
    uint8_t estatus;
    uint16_t transfer_count;
	uint8_t res3[2];
};

/*
** When the host receives an FIS command from a device it can be the union of any 
** one of the following...
**
** Note that they all begin with a common 4 bytes of padding followed by a type field...
**
*/

union __attribute__((packed)) Fis {
  __D2H    d2h;      // Typically request to wind down a transaction
  Activate activate; // DMA activate request
  Data     data;     // Data read from a device
  Setup    setup;    // PIO setup..
};

}}


#endif
