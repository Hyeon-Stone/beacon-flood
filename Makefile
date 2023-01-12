LDLIBS= -lpcap

all: beacon-flood

etc.o: etc.h etc.cpp hdr.h

parse.o: parse.h parse.cpp hdr.h


beacon-flood: main.o parse.o etc.o hdr.h
	$(LINK.cc) -w $^ $(LOADLIBES) $(LDLIBS) -o $@
	rm -f *.o
clean:
	rm -f arp-spoof *.o

