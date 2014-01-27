#include "CBitReader.h"
#include <cstdio>

CBitReader::CBitReader(const void *_buf, const unsigned _len)
{
  buf = static_cast<const unsigned char *>(_buf);
  len = _len;
  bits_read = 0;
  data_read = 0;
  offset = 0;
}

unsigned CBitReader::get(unsigned numbits)
{
  if (numbits>32)
  {
    #ifndef NDEBUG
    puts("BitReader: get(n>32)");
    #endif
     throw OutOfBounds();
  }
  while(bits_read < numbits)
  {
    if (offset>=len)
    {
      #ifndef NDEBUG
      puts("BitReader: get(): offset>=len");
      #endif
      throw OutOfBounds();
    }
    data_read = (data_read<<8) | buf[offset];
    bits_read+=8;
    offset++;
  }
  bits_read-=numbits;
  unsigned res =  (data_read>>bits_read) & ((1<<numbits)-1);
  //std::printf("--- get(%d)=%X\n", numbits, res);
  return res;
}

bool CBitReader::empty() const
{
  return ((bits_read==0) && (offset>=len));
}

bool CBitReader::byte_empty() const
{
  return (offset>=len);
}

bool CBitReader::is_all_zeroes() const
{
  for(unsigned i = offset; i<len; i++)
     if (buf[i]!=0)
        return false;
  return true;
}
