#include <stdlib.h>
#include <getopt.h>

#include "dvi/Endian.hh"

#include "reb/Bitfile.hh"

static const char USAGE_REB_FW_DUMP[] = 
  "\nREB Bitfile Dump\n"\
  "Usage: reb_fw_dump -f <file> -o <off> -l <len>\n"\
  "  -f <file>   The bitfile to load.\n"\
  "  -b          Dump bytes not words\n"\
  "  -o <off>    The offset in bytes into the bitfile to start\n"\
  "  -l <len>    The number of bytes to dump \n\n";


static void _dump_usage(void)
{
  printf("%s", USAGE_REB_FW_DUMP);
}

int main(int argc, char** argv)
{
  REB::Bitfile* file = NULL;
  int offset = -1;
  int length = -1;
  int bytes = 0;

  int theOpt;
  while ((theOpt = getopt(argc, argv, "hf:o:l:b")) != EOF)
  {
    switch(theOpt)
    {
    case 'h':
      _dump_usage();
      return -1;
    case 'f':
      try 
      {
	if(file) delete file;
	file = new REB::Bitfile(optarg);
      }
      catch(OSA::FileException& exception)
      {
	printf("bad filename: %s\n", optarg);
	if(file) delete file;
	return -1;
      }
      break;
    case 'b':
      bytes = 1;
      break;
    case 'o':
      offset = atol(optarg);
      break;
    case 'l':
      length = atol(optarg);
      break;
    default:
      printf("bad argument\n");
      _dump_usage();
      if(file) delete file;
      return -1;
    }
  }
  
  if(NULL==file)
  {
    _dump_usage();
    return -1;
  }

  printf("offset %i\n",offset);

  file->dump();
  unsigned len;
  const uint32_t* bitstream = file->bitstream(len);
  if(offset<0) offset = 0;
  if(length<0) length = len;

  if(bytes)
  {
    uint8_t* bitstream8 = (uint8_t*)bitstream;
    unsigned pos8       = offset;
    unsigned remaining  = length;
    while(remaining--)
    {
      if(0 == pos8%16) printf("\n0x%08X: ", pos8);
      unsigned data = bitstream8[pos8++];
      printf("%02X ", data);
    }
    printf("\n");
  }
  else
  {
    unsigned pos32     = offset/4;
    unsigned remaining = length/4;
    while(remaining--)
    {
      if(0 == pos32%4) printf("\n0x%08X: ", pos32*4);
      unsigned data = DVI::Endian::local(bitstream[pos32++]);
      printf("%08X ", data);
    }
    printf("\n");
  }

  if(file) delete file;
  
  return 0;
}
