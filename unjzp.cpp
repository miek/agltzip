#include <stdio.h>
#include <string.h>
#include <util/BinFile.h>
#include "CAgltDecompressor.h"
#include "errors.h"

void main(int argc, char *argv[])
{
	if (argc!=3)
  	{
    		printf("Usage: %s <in.jzp> <out.bin>\n", argv[0]);
    		return;
  	}

  	unsigned insize;
  	unsigned char *inbuf = LoadBinFile(argv[1], &insize);
  	if (!inbuf)
  	{
    		puts("Can't open input file !");
    		return;
  	}

  	CAgltDecompressor adc;
  	int decomp_size = adc.get_decomp_size(inbuf);
  	unsigned char *outbuf = new unsigned char[decomp_size];
  	memset(outbuf, 0x55, decomp_size);
  try
  {
    adc.decompress(inbuf, insize, outbuf);
  }
  catch (Overflow)
  {
    puts("put overflow detected !");
  }
  catch (Misalign)
  {
    puts("buffer misalign detected !");
  }
  catch (NegativeDistanse)
  {
    puts("negative distanse detected !");
  }
  catch (OutOfBounds)
  {
    puts("read out of bounds detected !");
  }
  SaveBinFile(argv[2], outbuf, decomp_size);
  delete outbuf;
  delete inbuf;
}
