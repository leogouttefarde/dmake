
CPATH=~/data/charm-6.7.1
#CPATH=~/charm-6.7.1

CHARMC=${CPATH}/bin/charmc $(OPTS)

OBJS = Master.o Slave.o Parser.o Node.o
OBJSPARSER = Parseur.o

all: Make

Make: $(OBJS)
	$(CHARMC) -language charm++ -g -lboost_filesystem-mt -lboost_system -o $@ $(OBJS)

Make.decl.h: Make.ci Job.h File.h
	$(CHARMC) Make.ci

clean:
	rm -f *.decl.h *.def.h conv-host *.o main Make charmrun

Master.o: Master.cpp Master.hpp Make.decl.h Parser.hpp
	$(CHARMC) -g -c Master.cpp

Slave.o: Slave.cpp Slave.hpp Make.decl.h
	$(CHARMC) -g -c Slave.cpp

Parser.o: Parser.cpp Parser.hpp
	$(CHARMC) -g -c Parser.cpp

Node.o: Node.cpp Node.hpp
	$(CHARMC) -g -c -lboost_system -lboost_filesystem-mt Node.cpp

