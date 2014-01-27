#ifndef _CBITWRITER_H_INCLUDED
#define _CBITWRITER_H_INCLUDED

class CBitWriter
{
	unsigned char *buf;
        unsigned len;
        unsigned offset, save_offset;
        unsigned data_wr, save_data_wr;
        int bits_wr, save_bits_wr;
public:
        CBitWriter(void *_buf, const unsigned _len);
        void put(unsigned data, unsigned numbits);
        bool full() const;
        void flush();
        unsigned used() const { return offset; }
};

#include "errors.h"

#endif
