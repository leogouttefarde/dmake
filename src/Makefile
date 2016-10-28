
CPATH=~/data/charm-6.7.1
#CPATH=~/charm-6.7.1

CHARMC=${CPATH}/bin/charmc $(OPTS)

OBJS = Master.o Slave.o

all: Make

Make: $(OBJS)
	$(CHARMC) -language charm++ -o $@ $(OBJS)

Make.decl.h: Make.ci
	$(CHARMC) Make.ci

clean:
	rm -f *.decl.h *.def.h conv-host *.o Make charmrun

Master.o: Master.C Master.h Make.decl.h
	$(CHARMC) -c Master.C

Slave.o: Slave.C Slave.h Make.decl.h
	$(CHARMC) -c Slave.C

