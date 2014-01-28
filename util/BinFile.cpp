#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

unsigned char *LoadBinFile(const char *fname, unsigned *size)
{
	*size = 0;
	int hf = open(fname, O_RDONLY | O_BINARY);
	if (hf<0)
		return 0;
	*size = filelength(hf);
	unsigned char *buf = new unsigned char[*size];
	if (read(hf, buf, *size)!=*size)
	{
		delete buf;
		*size = 0;
		close(hf);
		return 0;
	}
	close(hf);
	return buf;
}

bool SaveBinFile(const char *fname, const void *data, unsigned size)
{
	int hf = open(fname, O_CREAT | O_TRUNC | O_RDWR | O_BINARY, S_IWRITE);
	if (hf<0)
		return false;
	if (write(hf, data, size)!=size)
	{
		close(hf);
		return false;
	}
	close(hf);
	return true;
}
