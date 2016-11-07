
CPATH=~/data/charm-6.7.1
#CPATH=~/charm-6.7.1

CHARMC=${CPATH}/bin/charmc $(OPTS)

OBJS = Master.o Slave.o

all: Make

Make: $(OBJS)
	$(CHARMC) -language charm++ -g -o $@ $(OBJS)

Make.decl.h: Make.ci
	$(CHARMC) Make.ci

clean:
	rm -f *.decl.h *.def.h conv-host *.o Make charmrun

Master.o: Master.cpp Master.hpp Make.decl.h
	$(CHARMC) -c Master.cpp

Slave.o: Slave.cpp Slave.hpp Make.decl.h
	$(CHARMC) -c Slave.cpp

