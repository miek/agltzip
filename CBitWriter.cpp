#include "CBitWriter.h"
#include <cstdio>

CBitWriter::CBitWriter(void *_buf, const unsigned _len)
{
  	buf = static_cast<unsigned char *>(_buf);
  	len = _len;
  	bits_wr = 0;
 	data_wr = 0;
  	offset = 0;
}

void CBitWriter::put(unsigned data, unsigned numbits)
{
	if (offset>=len)
     		throw Overflow();
  	if (numbits>32)
     		throw OutOfBounds();
  	if ((numbits+bits_wr)<32)
  	{
    		data_wr<<=numbits;
    		data_wr|=data;
    		bits_wr+=numbits;
    		return;
  	}
  	data_wr<<=32-bits_wr;
  	data_wr |= data>>(numbits-(32-bits_wr));
  	buf[offset++] = data_wr>>24;
  	buf[offset++] = data_wr>>16;
  	buf[offset++] = data_wr>>8;
  	buf[offset++] = data_wr;
  	bits_wr = numbits+bits_wr-32;
  	data_wr = data & ((1<<bits_wr)-1);
}

void CBitWriter::flush()
{
	if (bits_wr==0)
     		return;
  	if (offset>=len)
     		throw Overflow();
  	data_wr<<=32-bits_wr;
  	for(; bits_wr>0; bits_wr-=8, data_wr<<=8)
     		buf[offset++] = data_wr>>24;
}
