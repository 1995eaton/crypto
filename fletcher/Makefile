CCFLAGS=-O2 -march=native -std=c++14

all: fletcher16 fletcher32

fletcher16: fletcher16.cc
	g++ $(CCFLAGS) fletcher16.cc -o fletcher16

fletcher32: fletcher32.cc
	g++ $(CCFLAGS) fletcher32.cc -o fletcher32

clean:
	rm -f fletcher16 fletcher32
