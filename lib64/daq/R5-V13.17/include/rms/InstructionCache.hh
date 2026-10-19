
#ifndef RMS_INSTRUCTION_CACHE
#define RMS_INSTRUCTION_CACHE

#include "osa/File.hh"
#include "rms/Instruction.hh"
#include "rms/InstructionList.hh"

namespace RMS  {

class InstructionCache
{
public:
  InstructionCache(const char* pathname, bool readonly=true);
public:
  InstructionCache() = delete;
  InstructionCache(const InstructionCache&) = delete;
public:
  ~InstructionCache() {}
public:
  int append(const Instruction&);
public:
  int read(InstructionList&);
public:
  void rewind() {_offset = 0;}
public:
  unsigned number() {return (_file.size()/sizeof(Instruction));}
  void     dump(unsigned indent=0);
public:
  void terminate() {_file.truncate(_offset);}
private:
  OSA::File _file;
  unsigned  _offset;
  };

}

#endif

