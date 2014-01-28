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
  	if (hdr->decomp_size==0)
     		return 0;
  	
	unsigned char dist_width = hdr->dist_width;
	unsigned dist_max = 1<<dist_width;
	unsigned size_width = hdr->size_width>>4;
	unsigned min_size = hdr->size_width & 0x0F;

  	const unsigned char *ptr = static_cast<const unsigned char *>(inbuf);
  	CBitReader *in = new CBitReader(ptr+sizeof(JZPHDR), swapl(hdr->comp_size) - sizeof(JZPHDR));
  	
  	COutBuffer *out = new COutBuffer(outbuf, swapl(hdr->decomp_size), dist_max);

  	try 
  	{
  		while(!(out->full() || in->empty()))
  		{
    			if (in->get(1)==1)
    			{ 
    				unsigned char lit = in->get(8);
    				//printf("lit %02X\n", lit);
				out->put(lit);
			}
        		else
        		{ 
        			unsigned short dist = in->get(dist_width);
        			unsigned char size = in->get(size_width)+min_size;
        			//printf("repeat %04X[%02X]\n", dist, size);
        			out->repeat(dist, size);
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
	if ((argc!=3) && (argc!=4))
  	{
    		printf("Usage: %s <in.jzp> <out.bin> [out_rev.txt]\n", argv[0]);
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
	if (strncmp(hdr->magic, "AGLTZIP", 8))
	{
		puts("Not a JZP !");
		delete inbuf;
		return;
	}
  	unsigned ck = jzp_checksum(inbuf, swapl(hdr->comp_size));
  	if (ck!=0)
  	{
  		puts("Checksum error ! Input file is corrupt !");
		delete inbuf;
		return;
	}
	puts(hdr->revision);
	if (argc==4) //save revision
	{
		SaveBinFile(argv[3], hdr->revision, strlen(hdr->revision));
	}
	unsigned decomp_size = swapl(hdr->decomp_size);
  	unsigned char *outbuf = new unsigned char[decomp_size];
    	
    	puts("Decompressing...");
    	if (decompress(inbuf, insize, outbuf)!=decomp_size)
    	{
    		puts("Decompression error !");
	}
  	
  	ck = jzp_checksum(outbuf, decomp_size);
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
