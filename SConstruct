env = Environment(TARGET_ARCH='x86', CCFLAGS=["/MT", "-EHsc", "/nologo", "/DNDEBUG"])
#env = Environment(TARGET_ARCH='x86', CCFLAGS=["/MT", "-EHsc", "/nologo"])
env.Program("unjzp", ["unjzp.cpp", "jzp.cpp", "CBitReader.cpp", "COutBuffer.cpp"])
env.Program("packjzp", ["packjzp.cpp", "jzp.cpp", "CBitWriter.cpp"])
