#############################
# CS211 Makefile y86emul   #
#############################

COMPILER = gcc
CCFLAGS = --ansi --pedantic -Wall
#CCFLAGS = -Wall -g

all: y86emul y86dis

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
ifeq ($(DEBUG), TRUE)
	CCFLAGS += -g
endif

clean:
	rm -f y86emul
	rm -f y86dis
	rm -f *.o
	rm -f *.gch
	rm -f *.aux
	rm -f *.log
	rm -f *.pdf
