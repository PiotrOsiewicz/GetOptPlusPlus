IDIR =../include
CC=g++
CFLAGS=-I$(IDIR) -std=c++11 -fprofile-arcs -ftest-coverage
VPATH=../src

ODIR=../tests/obj
LDI =../lib

LIBS=-lcriterion

_DEPS = getopt++.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = getopt++.o gopp_tests.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) $< -c -o $@ 
gopp_tests: $(OBJ)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@ 
.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
