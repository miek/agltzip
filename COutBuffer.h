#ifndef _COUTBUFFER_H_INCLUDED
#define _COUTBUFFER_H_INCLUDED

class COutBuffer
{
  unsigned char *buf;
  unsigned len;
  unsigned offset;
public:
  COutBuffer(void *_buf, unsigned _len);
  void put(const unsigned char);
  void put(const unsigned char *, const unsigned);
  bool full() const;
  unsigned used() const { return offset; }
};

#include "errors.h"

#endif
