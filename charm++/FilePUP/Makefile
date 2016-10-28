
#CPATH=~/data/charm-6.7.1
CPATH=~/charm-6.7.1

CHARMC=${CPATH}/bin/charmc $(OPTS)

OBJS = FilePUP.o

all: FilePUP

FilePUP: $(OBJS)
	$(CHARMC) -language charm++ -o FilePUP $(OBJS)

FilePUP.decl.h: FilePUP.ci
	$(CHARMC)  FilePUP.ci

clean:
	rm -f *.decl.h *.def.h conv-host *.o FilePUP charmrun

FilePUP.o: FilePUP.C FilePUP.h File.h FilePUP.decl.h
	$(CHARMC) -c FilePUP.C
