CFLAGS=-I. -O2
DEPS = *.h
OBJ = CBitReader.o CBitWriter.o COutBuffer.o jzp.o util/BinFile.o

all: packjzp unjzp

%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CFLAGS)

packjzp: $(OBJ)
	$(CXX) -o $@ packjzp.cpp $^ $(CFLAGS)


unjzp: $(OBJ)
	$(CXX) -o $@ unjzp.cpp $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o packjzp unjzp
