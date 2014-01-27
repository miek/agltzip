#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CAgltDecompressor.h"
#include "CBitReader.h"
#include "COutBuffer.h"

typedef struct
{
	char revision[128];
	unsigned comp_size; //including header
	unsigned comp_checksum;
	unsigned decomp_size;
	unsigned decomp_checksum;
	char magic[8];
	unsigned char dist_width, size_width, unk0, unk1;
} JZPHDR;

unsigned swapd(unsigned val)
{
	return (val>>24) | ((val>>8) & 0x0000FF00) | ((val<<8) & 0x00FF0000) | (val<<24);
}

CAgltDecompressor::CAgltDecompressor()
{
}

CAgltDecompressor::~CAgltDecompressor()
{
}

int CAgltDecompressor::get_decomp_size(const void *inbuf)
{
	const JZPHDR *hdr = static_cast<const JZPHDR *>(inbuf);
	return swapd(hdr->decomp_size);
}

int CAgltDecompressor::decompress( const void *inbuf, const unsigned inlen, void *outbuf)
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
	unsigned out_size_min_threshold = (1<<size_width) + min_size;
	unsigned _1 = 1;
	unsigned short in_size_min_threshold = dist_width + size_width + 1;
	unsigned dict_size = dist_max+1 + out_size_min_threshold;

	unsigned char *dict = new unsigned char[dict_size];
	unsigned char *dict_tail = dict+dict_size-1;
	unsigned char *dict_head = dict;
	unsigned char *dict_window = dict;

#ifndef NDEBUG
	printf("dist_width=%d, size_width=%d, min_size=%d\n", dist_width, size_width, min_size);
#endif
  	const unsigned char *ptr = static_cast<const unsigned char *>(inbuf);
  	CBitReader *in = new CBitReader(ptr+sizeof(JZPHDR), swapd(hdr->comp_size) - sizeof(JZPHDR));
  	
  	COutBuffer *out = new COutBuffer(outbuf, swapd(hdr->decomp_size));

  	unsigned char lit = 0;
  	unsigned char size = 0;
  	unsigned short dist = 0;
  	bool get_lit = false;
  	while(!(out->full() || in->empty()))
  	{
    		if (in->get(1)==1)
    		{ 
#ifndef NDEBUG
      			puts("get literal");
#endif
      			lit = in->get(8);
			size = 1;
			get_lit = true;
		}
		else
		{ 
#ifndef NDEBUG
			puts("get dist/size");
#endif
			dist = in->get(dist_width);
			size = in->get(size_width)+min_size;
			get_lit = false;
		}
#ifndef NDEBUG
		printf("\tlit=%08X, dist=%d, size=%d\n", lit, dist, size);
#endif
		if (size==1)
		{
#ifndef NDEBUG
			puts("store literal");
#endif
			if (!get_lit)
				puts("!!!");
			out->put(lit);
			*dict_window = lit;
		}
		else
		{
#ifndef NDEBUG
			puts("dist copy");
#endif
			if (get_lit)
				puts("!!!");
			out->put(dict_head+dist, size);
			memcpy(dict_window, dict_head+dist, size);
		}
		dict_window+=size;
		//update ptrs
		if ((dict_window-dict_head)>=dist_max)
		{
			dict_head = dict_window-dist_max;
			if ((dict_head-dict)>=_1)
			{
				memmove(dict, dict_head, dist_max);
				dict_head = dict;
				dict_window = dict+dist_max;
			}
		}
  	}
  	puts("Done");
  	if (in->empty())
     		puts("  IN is empty");
  	else
     		printf("  IN used %X of %X\n", in->used(), swapd(hdr->comp_size) - sizeof(JZPHDR));
  	if (out->full())
     		puts("  OUT is full");
  	else
     		printf("  OUT used %X of %X\n", out->used(), swapd(hdr->decomp_size));
  	int res = out->used();
  	delete out;
  	delete in;
  	return res;
}
