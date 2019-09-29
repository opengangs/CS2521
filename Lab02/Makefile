# COMP2521 19t1 ... lab 02 makefile

CC	= 2521 3c

.PHONY: all
all: testList myed

DLList.o:	DLList.c DLList.h

testList: 	testList.o DLList.o
testList.o:	testList.c DLList.h

myed:		myed.o DLList.o
myed.o:		myed.c DLList.h

.PHONY: clean
clean:
	-rm -f DLList.o
	-rm -f testList testList.o
	-rm -f myed myed.o

.PHONY: give
give: DLList.c
	give cs2521 lab02 $^
