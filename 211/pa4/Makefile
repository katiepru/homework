#############################
# CS211 Makefile y86emul   #
#############################

COMPILER = gcc
CCFLAGS = --ansi --pedantic -Wall

all: y86emul

debug:
	make DEBUG=TRUE

y86emul: y86emul.o
	$(COMPILER) $(CCFLAGS) -o y86emul y86emul.o
y86emul.o: y86emul.c y86emul.h
	$(COMPILER) $(CFLAGS) -c y86emul.c y86emul.h

ifeq ($(DEBUG), TRUE)
	CCFLAGS += -g
endif

clean:
	rm -f y86emul
	rm -f *.o
	rm -f *.gch
	rm -f *.aux
	rm -f *.log
	rm -f *.pdf
