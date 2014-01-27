#ifndef _CAGLTDECOMPRESSOR_H_INCLUDED
#define _CAGLTDECOMPRESSOR_H_INCLUDED

class CAgltDecompressor
{
public:
  CAgltDecompressor();
  ~CAgltDecompressor();
  static int get_decomp_size(const void *inbuf);
  int decompress(const void *inbuf, const unsigned insize, void *outbuf);
};

#endif
