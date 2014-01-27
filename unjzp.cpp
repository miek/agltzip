#include <stdio.h>
#include <string.h>
#include <util/BinFile.h>
#include "jzp.h"
#include "CBitReader.h"
#include "COutBuffer.h"
#include "errors.h"

int decompress( const void *inbuf, const unsigned inlen, void *outbuf)
{
	const JZPHDR *hdr = static_cast<const JZPHDR *>(inbuf);
	if (strncmp(hdr->magic, "AGLTZIP", 8))
	{
		puts("Not a JZP !");
		return 0;
	}
  	if (hdr->decomp_size==0)
     		return 0;
  	
	unsigned char dist_width = hdr->dist_width;
	unsigned dist_max = 1<<dist_width;
	unsigned size_width = hdr->size_width>>4;
	unsigned min_size = hdr->size_width & 0x0F;

	//unsigned out_size_min_threshold = (1<<size_width) + min_size;
	//unsigned short in_size_min_threshold = dist_width + size_width + 1;
	unsigned dict_size = dist_max+1 + (1<<size_width)+min_size;

	unsigned char *dict = new unsigned char[dict_size];
	//unsigned char *dict_tail = dict+dict_size-1;
	unsigned char *dict_head = dict;
	unsigned char *dict_window = dict;

	#ifndef NDEBUG
	printf("dist_width=%d, size_width=%d, min_size=%d\n", dist_width, size_width, min_size);
	#endif
  	const unsigned char *ptr = static_cast<const unsigned char *>(inbuf);
  	CBitReader *in = new CBitReader(ptr+sizeof(JZPHDR), swapl(hdr->comp_size) - sizeof(JZPHDR));
  	
  	COutBuffer *out = new COutBuffer(outbuf, swapl(hdr->decomp_size));

  	try 
  	{
  		while(!(out->full() || in->empty()))
  		{
    			if (in->get(1)==1)
    			{ 
				#ifndef NDEBUG
      				puts("store literal");
				#endif
      				unsigned char lit = in->get(8);
				out->put(lit);
				*dict_window = lit;
				dict_window++;
			}
        		else
        		{ 
        			#ifndef NDEBUG
        			puts("dist copy");
        			#endif
        			unsigned short dist = in->get(dist_width);
        			unsigned char size = in->get(size_width)+min_size;
        			out->put(dict_head+dist, size);
        			memcpy(dict_window, dict_head+dist, size);
        			dict_window+=size;
        		}
        		//rewind dict circular buffer if nessesary
        		if ((dict_window-dict_head)>=dist_max)
        		{
        			dict_head = dict_window-dist_max;
        			if ((dict_head-dict)>=1)
        			{
        				memmove(dict, dict_head, dist_max);
        				dict_head = dict;
        				dict_window = dict+dist_max;
        			}
        		}
          	}
  	}
        catch (Overflow)
        {
          	puts("Output overflow detected !");
        }
        catch (Misalign)
        {
          	puts("Buffer misalign detected !");
        }
        catch (NegativeDistanse)
        {
          	puts("Negative distanse detected !");
        }
        catch (OutOfBounds)
        {
          	puts("Out of bounds !");
        }
  	
  	if (in->byte_empty() && out->full())
  	{
  		puts("LZSS: OK");
	}
	else
	{
  		if (in->empty())
     			puts("  IN is empty");
  		else
     			printf("  IN used %X of %X\n", in->used(), swapl(hdr->comp_size) - sizeof(JZPHDR));
  		if (out->full())
     			puts("  OUT is full");
  		else
     			printf("  OUT used %X of %X\n", out->used(), swapl(hdr->decomp_size));
  	}
  	int res = out->used();
  	delete out;
  	delete in;
  	return res;
}

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

  	const JZPHDR *hdr = (const JZPHDR *)inbuf;
	unsigned decomp_size = swapl(hdr->decomp_size);

  	unsigned char *outbuf = new unsigned char[decomp_size];
    	
    	if (decompress(inbuf, insize, outbuf)!=decomp_size)
    	{
    		puts("Decompression error !");
	}
  	
  	unsigned ck = jzp_checksum(outbuf, decomp_size);
  	if (ck==hdr->decomp_checksum)
  	{
  		puts("CHECKSUM: OK");
	}
	else
	{
  		printf("CHECKSUM: FAIL\ncalc checksum: %08X, hdr checksum: %08X\n", swapl(ck), swapl(hdr->decomp_checksum));
	}
  	SaveBinFile(argv[2], outbuf, decomp_size);
  	
  	delete outbuf;
  	delete inbuf;
}
