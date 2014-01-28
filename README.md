# Agilent JZP tools

A set of tools for Agilent JZP compression format.  
A well-known LZSS algorithm with custom header is used.  

Compression speed is far from being optimal, compressing a 10MiB file takes around 1 minute on i7 machine, but it works :)  
Bugfixed compressor produces same files as originals now.  
Example:  
	unjzp sys.jzp sys.bin sys_rev.txt
	packjzp sys.bin sys_new.jzp sys_rev.txt
sys_new.jzp will match sys.jzp exactly

