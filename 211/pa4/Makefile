#############################
# CS211 Makefile y86emul   #
#############################

COMPILER = gcc
CCFLAGS = --ansi --pedantic -Wall
#CCFLAGS = -Wall -g

all: y86emul y86dis y86ass

debug:
	make DEBUG=TRUE

y86emul: y86emul.o
	$(COMPILER) $(CCFLAGS) -o y86emul y86emul.o
y86emul.o: y86emul.c y86emul.h
	$(COMPILER) $(CFLAGS) -c y86emul.c y86emul.h
y86dis: y86dis.o
	$(COMPILER) $(CCFLAGS) -o y86dis y86dis.o
y86dis.o: y86dis.c y86dis.h
	$(COMPILER) $(CFLAGS) -c y86dis.c y86dis.h
y86ass: y86ass.o
	$(COMPILER) $(CCFLAGS) -o y86ass y86ass.o
y86ass.o: y86ass.c y86ass.h
	$(COMPILER) $(CFLAGS) -c y86ass.c y86ass.h
ifeq ($(DEBUG), TRUE)
	CCFLAGS += -g
endif

clean:
	rm -f y86emul
	rm -f y86dis
	rm -f y86ass
	rm -f *.o
	rm -f *.gch
	rm -f *.aux
	rm -f *.log
	rm -f *.pdf
