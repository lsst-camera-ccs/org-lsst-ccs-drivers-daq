#ifndef REB_REGMAP
#define REB_REGMAP

#include <stdint.h>

// Note that the source of these constants and register addresses
// derive from both the REB firmware _and_ the conventions of the
// Sequencer program and may need updating or changing if those
// sources change.

namespace REB {

typedef struct RebControl {
  uint32_t startBase;
  uint32_t stopBase;
  uint32_t stepBase;
  uint32_t state;
  uint32_t lamMask;
  uint32_t idHigh;
  uint32_t idLow;
  uint32_t version;} RebControl;

static const RebControl REB_ADDR = {0x340000,  // startBase
				    0x320000,  // stopBase
				    0x310000,  // stepBase
				    0x000008,  // state
				    0x000017,  // lamMask
				    0x800001,  // idHigh
				    0x800002,  // idLow
				    0x000001}; // version

typedef struct SequencerParams {
  uint32_t preCols;
  uint32_t underCols;
  uint32_t readCols;
  uint32_t postCols;
  uint32_t readCols2;
  uint32_t overCols;
  uint32_t parallelBin;
  uint32_t preRowsG;
  uint32_t postRowsG;
  uint32_t preRows;
  uint32_t readRows;
  uint32_t postRows;
  uint32_t overRows;
  uint32_t clearCount;
  uint32_t binnedParLen;
  uint32_t shiftCount;
  uint32_t pumpNumber;
  uint32_t flushCount;
  uint32_t opFlags;
} SequencerParams;

static const SequencerParams SEQ_ADDR = {0x360000,  // PreCols
					 0x360001,  // UnderCols
					 0x360002,  // ReadCols
					 0x360003,  // PostCols
					 0x360004,  // ReadCols2
					 0x360005,  // OverCols
					 0x360006,  // ParallelBin
					 0x360007,  // PreRowsG
					 0x360008,  // PostRowsG
					 0x380000,  // PreRows
					 0x380001,  // ReadRows
					 0x380002,  // PostRows
					 0x380003,  // OverRows
					 0x380004,  // ClearCount
					 0x380005,  // BinnedParLen
					 0x380006,  // ShiftCount
					 0x380007,  // PumpNumber
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

static const EEpromControl EEPROM_ADDR = {0x000100,  // slot - start
					  0x000101,  // data - we
					  0x000102,  // done - done
					  0x000103,  // stat - status read
					  0x000200}; // boot - multiboot

}
#endif
