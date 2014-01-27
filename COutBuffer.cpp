#include <cstdio>
#include <string.h>
#include "COutBuffer.h"

COutBuffer::COutBuffer(void *_buf, unsigned _len)
{
  buf = static_cast<unsigned char *>(_buf);
  len = _len;
  offset = 0;
}

void COutBuffer::put(const unsigned char b)
{
  if (offset>=len)
     throw Overflow();
  buf[offset++] = b;
  //std::printf("--- put %04X\n", w);
}

void COutBuffer::put(const unsigned char *src, const unsigned size)
{
  	if (offset>=len)
     		throw Overflow();
  	if ((offset+size)>=len)
     		throw OutOfBounds();
	memcpy(buf+offset, src, size);  	
  	offset+=size;
}

bool COutBuffer::full() const
{
  return (offset>=len);
}
