CC=g++
CFLAGS=
LIBS=

.PHONY: all
all: software_vt
software_vt: software_vt.cpp RDTSCReference.cpp SharedIncludes.h
	g++ -g -O0 -std=c++11 -DLINUX software_vt.cpp RDTSCReference.cpp -lpthread -o software_vt

.PHONY: clean
clean:
	rm -rf software_vt
