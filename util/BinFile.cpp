#include <fstream>

unsigned char *LoadBinFile(const char *fname, unsigned *size)
{
	std::ifstream file(fname, std::ios::binary | std::ios::ate);
	std::streamsize s = file.tellg();
	file.seekg(0, std::ios::beg);

	char *buf = new char[s];
	if (file.read(buf, s))
	{
		*size = s;
		return (unsigned char*)buf;
	}
	delete buf;
	return 0;
}

bool SaveBinFile(const char *fname, const void *data, unsigned size)
{
	std::ofstream file(fname, std::ios::binary | std::ios::trunc);
	file.write((const char*)data, size);
}
