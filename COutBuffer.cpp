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
  	{
  		#ifndef NDEBUG
  		printf("put(%02X) overflow: offset=%X, len=%X\n", b, offset, len);
  		#endif
     		throw Overflow();
	}
  	buf[offset++] = b;
  //std::printf("--- put %04X\n", w);
}

void COutBuffer::put(const unsigned char *src, const unsigned size)
{
  	if (offset>=len)
  	{
  		#ifndef NDEBUG
  		printf("put(src,%X) overflow: offset=%X, len=%X\n", size, offset, len);
  		#endif
     		throw Overflow();
	}
  	if ((offset+size)>len)
  	{
  		#ifndef NDEBUG
  		printf("put(src,%X) out of bounds: offset=%X, len=%X\n", size, offset, len);
  		#endif
     		throw OutOfBounds();
	}
	memcpy(buf+offset, src, size);  	
  	offset+=size;
}

bool COutBuffer::full() const
{
	return (offset>=len);
}
