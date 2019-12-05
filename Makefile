CC  = gcc
SRC = $(shell find . -name "*.c")
OBJ = $(addsuffix .o,$(basename $(SRC)))

DEP = sdl2

CFLAG = -Iinclude `pkg-config $(DEP) --cflags`
LFLAG = `pkg-config $(DEP) --libs` -fopenmp
EXE   = gol

.SUFFIXES: .o .c
.PHONY: all test clean

all: $(EXE)

test: all
	./$(EXE)

clean:
	rm $(OBJ)

$(EXE): $(OBJ)
	$(CC) -o $(EXE) $(OBJ) $(LFLAG)

.c.o:
	$(CC) -c -o $@ $< $(CFLAG)
