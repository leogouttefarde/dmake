
CPATH=~/data/charm-6.7.1
#CPATH=~/charm-6.7.1

CHARMC=${CPATH}/bin/charmc $(OPTS)

OBJS = Master.o Slave.o Parser.o
OBJSPARSER = Parseur.o
all: Make

parser: $(OBJSPARSER)
	g++ Main.cpp -o main $^

Make: $(OBJS)
	$(CHARMC) -language charm++ -g -o $@ $(OBJS)

Make.decl.h: Make.ci
	$(CHARMC) Make.ci

clean:
	rm -f *.decl.h *.def.h conv-host *.o main Make charmrun

Master.o: Master.cpp Master.hpp Make.decl.h Parser.hpp
	$(CHARMC) -g -c Master.cpp

Slave.o: Slave.cpp Slave.hpp Make.decl.h
	$(CHARMC) -g -c Slave.cpp

Parser.o: Parser.cpp Parser.hpp
	$(CHARMC) -g -c Parser.cpp

Parseur.o: Parseur.cpp
	g++ -o $@ -c $<
