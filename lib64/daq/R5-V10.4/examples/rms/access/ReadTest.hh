#ifndef RMS_ACCESS_READTEST
#define RMS_ACCESS_READTEST

#include "rms/Harvest.hh"
#include "Command.hh"

namespace DAQ { class Location; }

namespace RMS { class Client; class InstructionList;

namespace Access {

class ReadTest : public Command { 

class Harvest : public RMS::Harvest
{
public:
  Harvest(ReadTest& test) : _test(test) {};
public:
  void process(const DAQ::Location&, const InstructionList&, int32_t error);
public:
  uint8_t  nreg;
private:
  ReadTest& _test;
};

friend class Harvest;

public:
  ReadTest(Client&);
public:
 ~ReadTest() {}
public:
  void process(int argc, const char** argv, int qualifier);
  void help(   int argc, const char** argv, int qualifier);        
public:
  static void usage();
public:
  void printStats(int failed);
private:
  int _parse_flags(int argc, const char**argv);
  void _loop(const DAQ::LocationSet&, const InstructionList&);
private:
  Harvest _harvest;
public:
  bool     _do_exit;
  bool     _do_dump;
  bool     _ignore;
  bool     _printem;
  unsigned _cadence;
  unsigned _ucadence;
public:
  time_t _begin_time;
  time_t _end_time;
private:
  long long unsigned _requests;
  long long unsigned _responses;
  long long unsigned _response_errors;
  long long unsigned _instructions;
  long long unsigned _instruction_errors;
  long long unsigned _instruction_count_errors;
};

}}

#endif
