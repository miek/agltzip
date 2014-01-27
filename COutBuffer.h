#ifndef _COUTBUFFER_H_INCLUDED
#define _COUTBUFFER_H_INCLUDED

class COutBuffer
{
	unsigned char *buf;
        unsigned len;
        unsigned offset;
        unsigned window_size;
        void adjust_dict();
public:
        COutBuffer(void *_buf, unsigned _len, unsigned _maxdist);
        void put(const unsigned char);
      	void repeat(const unsigned short dist, const unsigned char size);
        bool full() const;
        unsigned used() const { return offset; }
};

#include "errors.h"

#endif
