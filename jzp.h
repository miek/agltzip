#ifndef _JZP_H_INCLUDED
#define _JZP_H_INCLUDED

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

unsigned swapl(unsigned u); //minimalistic htonl/ntohl for windows
unsigned jzp_checksum(const void *buf, unsigned size);

#endif
