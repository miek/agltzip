#ifndef _BINFILE_H_INCLUDED
#define _BINFILE_H_INCLUDED

#pragma message("Using library: BinFile")
#pragma comment(lib, "BinFile")

unsigned char *LoadBinFile(const char *fname, unsigned *size);
bool SaveBinFile(const char *fname, const void *data, unsigned size);

#endif
