#include "jzp.h"

unsigned swapl(unsigned u)
{
	return (u>>24) | ((u>>8) & 0x0000FF00) | ((u<<8) & 0x00FF0000) | (u<<24);
}

unsigned jzp_checksum(const void *buf, unsigned size)
{
	const unsigned *ptr = static_cast<const unsigned *>(buf);
	unsigned sum = 0;
	for(unsigned u=0; u<(size>>2); u++)
	{
		sum += swapl(ptr[u]);
	}
	return swapl(-sum);
}
