#ifndef REB_REGMAP
#define REB_REGMAP

#include <stdint.h>

// Note that the source of these constants and register addresses
// derive from both the REB firmware _and_ the conventions of the
// Sequencer program and may need updating or changing if those
// sources change.

namespace REB {

typedef struct RebControl {
  uint32_t progMemBase;
  uint32_t startBase;
  uint32_t stopBase;
  uint32_t stepBase;
  uint32_t state;
  uint32_t lamMask;
  uint32_t idHigh;
  uint32_t idLow;
  uint32_t version;
  uint32_t ccdOE;} RebControl;

static const RebControl REB_ADDR = {
  0x300000,  // progMemBase
  0x340000,  // startBase
  0x320000,  // stopBase
  0x310000,  // stepBase
  0x000008,  // state
  0x000017,  // lamMask
  0x800001,  // idHigh
  0x800002,  // idLow
  0x000001,  // version
  0x400008}; // ccdOE

typedef struct SequencerParams {
  uint32_t preCols;
  uint32_t underCols;
  uint32_t readCols;
  uint32_t postCols;
  uint32_t readCols2;
  uint32_t overCols;
  uint32_t preRowsG;
  uint32_t postRowsG;
  uint32_t preRows;
  uint32_t readRows;
  uint32_t postRows;
  uint32_t overRows;
  uint32_t flushCount;
  uint32_t opFlags;
} SequencerParams;

static const SequencerParams SEQ_ADDR = {
  0x360000,  // PreCols
  0x360001,  // UnderCols
  0x360002,  // ReadCols
  0x360003,  // PostCols
  0x360004,  // ReadCols2
  0x360005,  // OverCols
  0x360007,  // PreRowsG
  0x360008,  // PostRowsG
  0x380000,  // PreRows
  0x380001,  // ReadRows
  0x380002,  // PostRows
  0x380003,  // OverRows
  0x380008,  // FlushCount
  0x380009}; // OpFlags

enum SequencerFlags : unsigned {FLAG_ITL=1, FLAG_E2V=2};

typedef struct EEpromControl {
  uint32_t slot;
  uint32_t data;
  uint32_t done;
  uint32_t stat;
  uint32_t boot;
} EEpromControl;

static const EEpromControl EEPROM_ADDR = {
  0x000100,  // slot - start
  0x000101,  // data - we
  0x000102,  // done - done
  0x000103,  // stat - status read
  0x000200}; // boot - multiboot

typedef struct SciStatusBlock {
  uint32_t buildStringBase;
  uint32_t gitHashBase;
  uint32_t fwVersion;
  uint32_t clockPeriod;
  uint32_t locationBase;
} SciStatusBlock;

static const SciStatusBlock SCI_STATUS_ADDR = {
  0xA00100,  // buildStringBase (64 registers, 256 bytes ASCII)
  0xA00140,  // gitHashBase     (5 registers, 160-bit SHA-1)
  0xA00145,  // fwVersion
  0x000020,  // clockPeriod     (picoseconds, 0 = fixed 10ns)
  0xA00200}; // locationBase    (4 registers, 128-bit location mask)

typedef struct PgpStatusBlock {
  uint32_t linkStatus;
  uint32_t remLinkData;
  uint32_t cellErrors;
  uint32_t linkDown;
  uint32_t linkErrors;
  uint32_t remOverflowBase;   // 4 registers (VC0-3)
  uint32_t rxErrors;
  uint32_t rxFrames;
  uint32_t locOverflowBase;   // 4 registers (VC0-3)
  uint32_t txErrors;
  uint32_t txFrames;
  uint32_t opcodeCount;
  uint32_t opcodeLast;
  uint32_t noticeCount;
  uint32_t noticeLast0;
  uint32_t noticeLast1;
  uint32_t pgpSide;
} PgpStatusBlock;

static const PgpStatusBlock PGP_STATUS_ADDR = {
  0xA00001,  // linkStatus      (composite: ready/overflow/pause/polarity)
  0xA00002,  // remLinkData     (bits [7:0])
  0xA00003,  // cellErrors
  0xA00004,  // linkDown
  0xA00005,  // linkErrors
  0xA00006,  // remOverflowBase (4 registers: VC0=0x06, VC1=0x07, VC2=0x08, VC3=0x09)
  0xA0000E,  // rxErrors
  0xA0000F,  // rxFrames
  0xA00010,  // locOverflowBase (4 registers: VC0=0x10, VC1=0x11, VC2=0x12, VC3=0x13)
  0xA00018,  // txErrors
  0xA00019,  // txFrames
  0xA00020,  // opcodeCount     (RX opcode receive count)
  0xA00021,  // opcodeLast      (last received opcode, bits [7:0])
  0xA00022,  // noticeCount     (TX notice sent count)
  0xA00023,  // noticeLast0     (last notice bits [31:0])
  0xA00024,  // noticeLast1     (last notice bits [63:32])
  0xA00025}; // pgpSide         (bit 1 = side B, bit 0 = side A)

static const uint32_t PGP_STATUS_RESET = 0xA00000;  // write to clear PGP counters

typedef struct ImageStatusBlock {
  uint32_t sentBase;     // 3 registers (VC0, VC1, VC2) at +0x00 stride 0x10
  uint32_t discBase;     // 3 registers (VC0, VC1, VC2) at +0x01 stride 0x10
  uint32_t truncBase;    // 3 registers (VC0, VC1, VC2) at +0x02 stride 0x10
  uint32_t formatBase;   // 3 registers (VC0, VC1, VC2) at +0x03 stride 0x10
} ImageStatusBlock;

static const unsigned IMAGE_VC_STRIDE = 0x10;
static const unsigned IMAGE_NUMOF_VCS = 3;

static const ImageStatusBlock IMAGE_STATUS_ADDR = {
  0xA00030,  // sentBase   (VC0=0x30, VC1=0x40, VC2=0x50)
  0xA00031,  // discBase   (VC0=0x31, VC1=0x41, VC2=0x51)
  0xA00032,  // truncBase  (VC0=0x32, VC1=0x42, VC2=0x52)
  0xA00033}; // formatBase (VC0=0x33, VC1=0x43, VC2=0x53)

}
#endif
