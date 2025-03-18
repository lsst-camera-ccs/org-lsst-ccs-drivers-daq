#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <csignal>
#include <ctime>
#include <unistd.h>

#include "osa/Tokens.hh"
#include "rms/Client.hh"
#include "rms/Errors.hh"

#include "ReadTest.hh"

using namespace RMS;

static const char COMMAND[] = "readtest";


static Access::ReadTest* _test;


extern "C" {
  void _quit_signal(int s)
  {
    printf("\n"); 
    if(_test->_do_exit)
    {
      printf("\n");
      _test->_end_time = time(NULL);
      _test->printStats(1);
      exit(1);
    }
    _test->_do_exit=true;
  };
};



/*
** ++
**
**
** --
*/

Access::ReadTest::ReadTest(Client& client) :
  Command(COMMAND, client),
  _harvest(*this),
  _do_exit(false),
  _do_dump(false),
  _ignore(false),
  _printem(false),
  _cadence(0),
  _ucadence(0),
  _requests(0),
  _responses(0),
  _response_errors(0),
  _instructions(0),
  _instruction_errors(0),
  _instruction_count_errors(0)
{ 
  _test = this;
  
  struct sigaction sigIntHandler;
  sigIntHandler.sa_handler = _quit_signal;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;
  sigaction(SIGINT, &sigIntHandler, NULL);

}

/*
** ++
**
**
** --
*/

#define REGISTERS argv[optind++]
#define TARGETS   argv[optind++]

static const char NO_REGISTER[] = "No Valid Register Address supplied\n";
static const char NO_TARGETS[]  = "No valid targets given\n";
static const uint32_t DONT_CARE = 0;

void Access::ReadTest::process(int argc, const char** argv, int constant)   
{
  
  int arg = _parse_flags(argc, argv);
  if(0 == argc-arg)  { printf(NO_REGISTER); return; }

  unsigned base;
  unsigned number;
  if(!_parse_regs(argv[arg++], base, number)) { printf(NO_REGISTER); return; }
  
  _harvest.nreg = number;
  InstructionList instructions(number);
  _build_list(base, Instruction::GET, DONT_CARE, instructions);

  DAQ::LocationSet targets(argc-arg, (const char**)&argv[arg]);
  if(!targets) { printf(NO_TARGETS); return; }

  _loop(targets, instructions);

  return; 
}


void Access::ReadTest::printStats(int failed)
{

  double delta_time = difftime(_end_time, _begin_time);

  double requests_per_second = _requests/delta_time;
  double instructions_per_second = _instructions/delta_time;
  double instructions_per_request = ((double)_instructions)/_requests;
  double usec_per_request = (delta_time/_requests)*1000000.;
  double usec_per_instruction = (delta_time/_instructions)*1000000.;

  printf("**************************************************\n");
  printf(" RMS Register Read Stress Test Summary\n");
  printf("**************************************************\n");
  printf("%22s: %.0f seconds\n","Test Duration", delta_time);
  printf("**************************************************\n");
  printf("%24s: %llu\n", "Requests", _requests);
  printf("%24s: %llu\n", "Responses", _responses);
  printf("%24s: %llu\n", "Response Errors", _response_errors);
  printf("%24s: %llu\n", "Instructions", _instructions);
  printf("%24s: %llu\n", "Instruction Errors", _instruction_errors);
  printf("%24s: %llu\n", "Instruction Count Errors", _instruction_count_errors);
  printf("**************************************************\n");
  printf("%24s: %f\n", "Requests/sec", requests_per_second);
  printf("%24s: %f\n", "Instructions/sec", instructions_per_second);
  printf("%24s: %f\n", "Instructions/Request", instructions_per_request);
  printf("%24s: %f\n", "usec/Request", usec_per_request);
  printf("%24s: %f\n", "usec/Instruction", usec_per_instruction);
  printf("**************************************************\n");

}

/*
** ++
**
**
** --
*/

static const char HELP[] = "rms_read_test [-p] [-i] [-c cadence] [-u ucadence] <partition> <reg>[:num] [DAQ Locations]\n" \
  "  -p  print      print results read periodically\n"			\
  "  -i  ignore     Ignore errors and continue\n"			\
  "  -c  cadence    Cadence between acquisitions (default=0 seconds)\n" \
  "  -u  ucadence   Cadence between acquisitions (default=0 microseconds)\n\n";

void Access::ReadTest::usage()
{
  printf(HELP);
  DAQ::LocationSet::usage();
}

void Access::ReadTest::help(int argc, const char** argv, int qualifier)   
{
  usage();
}


int Access::ReadTest::_parse_flags(int argc, const char** argv)
{
  int arg = 0;
  while(arg<argc)
  {
    if     (0 == strcmp("-c", argv[arg]))  _cadence = atol(argv[++arg]); 
    else if(0 == strcmp("-u", argv[arg])) _ucadence = atol(argv[++arg]);
    else if(0 == strcmp("-p", argv[arg]))  _printem = true;
    else if(0 == strcmp("-i", argv[arg]))   _ignore = true;
    else break;
    ++arg;
  }
 
  return arg;
}


void Access::ReadTest::_loop(const DAQ::LocationSet& targets, const InstructionList& instructions)
{
  unsigned dot = _cadence ? 1 : 10000;
  unsigned ndots = 0;

  _begin_time = time(NULL);
  while(!_do_exit)
  {
    _client.access(targets, instructions, _harvest);
    ++_requests;

    if(_do_dump) _do_dump = false;

    if(_requests%dot==0)
    {
      if(ndots++%10 == 0) { printf("%i",ndots); fflush(stdout); }
      else                { printf(".");        fflush(stdout); }
      if(_printem) _do_dump = true;
    }
    
    if(!_do_exit)
    {
      if      (_cadence)  sleep( _cadence);
      else if(_ucadence) usleep(_ucadence);
    }
    
  }

  _end_time = time(NULL);
  printStats(0);
}


static const char ERROR[]         = "%4s:        ERROR: %s (%i)\n";

void Access::ReadTest::Harvest::process(const DAQ::Location& location, const InstructionList& response, int32_t error)
{
  char source[8];
  location.encode(source);

  ++_test._responses;

  if(error) 
  {
    ++_test._response_errors;
    if(!_test._ignore) 
    {
      _test._do_exit = true; 
      printf(ERROR, source, Errors::decode(error), error); 
    }
    return;
  }
  
  if(nreg != response.level())
  {
    printf("Response didn't contain the correct number of instructions\n");
    ++_test._instruction_count_errors;
    if(!_test._ignore) { _test._do_exit = true; response.dump(); }
    return;
  }
  
  _test._instructions += response.level();
  
  for (unsigned i=0; i < response.level(); ++i)
  {
    if(response.lookup(i)->fault())
    {
      ++_test._instruction_errors;
      if(!_test._ignore) { _test._do_exit = true; response.dump(); }
      return;
    }
  }

  if(_test._do_dump) response.dump();

  return;
}
