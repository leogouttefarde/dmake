
CPATH=~/data/charm-6.7.1
#CPATH=~/charm-6.7.1

CHARMC=${CPATH}/bin/charmc $(OPTS)

OBJS = Master.o Slave.o Parser.o Node.o
OBJSPARSER = Parseur.o
#DEBUG=-g

all: Make

Make: $(OBJS)
	$(CHARMC) -language charm++ ${DEBUG} -o $@ $(OBJS)

Make.decl.h: Make.ci Job.h File.h
	$(CHARMC) Make.ci

clean:
	rm -f *.decl.h *.def.h conv-host *.o main Make charmrun

Master.o: Master.cpp Master.hpp Make.decl.h Parser.hpp
	$(CHARMC) ${DEBUG} -c Master.cpp

Slave.o: Slave.cpp Slave.hpp Make.decl.h
	$(CHARMC) ${DEBUG} -c Slave.cpp

Parser.o: Parser.cpp Parser.hpp
	$(CHARMC) ${DEBUG} -c Parser.cpp

Node.o: Node.cpp Node.hpp
	$(CHARMC) ${DEBUG} -c Node.cpp

