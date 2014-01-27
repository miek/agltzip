#include <cstdio>
#include <string.h>
#include "COutBuffer.h"

COutBuffer::COutBuffer(void *_buf, unsigned _len, unsigned _winsize)
{
  buf = static_cast<unsigned char *>(_buf);
  len = _len;
  offset = 0;
  window_size = _winsize;
}

void COutBuffer::put(const unsigned char b)
{
  	if (offset>=len)
  	{
  		#ifndef NDEBUG
  		printf("put(%02X) overflow: offset=%X, len=%X\n", b, offset, len);
  		#endif
     		throw Overflow();
	}
  	buf[offset++] = b;
}

void COutBuffer::repeat(const unsigned short dist, const unsigned char size)
{
  	if (offset>=len)
     		throw Overflow();
  	if (dist>=offset)
     		throw NegativeDistanse();
	memcpy(buf+offset, buf+((offset>=window_size) ? (offset+dist-window_size) : dist), size);  	
  	offset+=size;
}

bool COutBuffer::full() const
{
	return (offset>=len);
}
