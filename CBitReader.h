#ifndef _CBITREADER_H_INCLUDED
#define _CBITREADER_H_INCLUDED

class CBitReader
{
  const unsigned char *buf;
  unsigned len;
  unsigned offset;
  unsigned data_read;
  unsigned bits_read;
public:
  CBitReader(const void *_buf, const unsigned _len);
  unsigned get(unsigned numbits);
  bool empty() const; //strict empty (no more bits)
  bool byte_empty() const; //no more bytes
  unsigned used() const { return offset; }
  bool is_all_zeroes() const;
};

#include "errors.h"

#endif
