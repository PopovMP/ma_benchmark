OBJDIR = ./obj
SRCDIR = ./src
LIBDIR = ./lib

CC    = /usr/bin/gfortran
WARN  = -Wall -Wextra -Wpedantic
STD   = -std=f2008
ARCH  = -march=x86-64-v3
OPT   = -Ofast -ffast-math -ftree-vectorize -finline-functions

BIN   = ./main

OBJECTS = $(OBJDIR)/data-set.o       \
          $(OBJDIR)/shared-state.o   \
          $(OBJDIR)/moving-average.o \
          $(OBJDIR)/rsi.o            \
          $(OBJDIR)/std-dev.o

.PHONY : all
all : $(BIN)

$(BIN) : $(OBJECTS)
	$(CC) $(WARN) $(STD) $(ARCH) $(OPT) -o $@ $^ $(SRCDIR)/$(BIN).f90

$(OBJDIR)/%.o : $(LIBDIR)/%.f90
	$(CC) $(WARN) $(STD) $(ARCH) $(OPT) -c -o $@ $<

.PHONY : clean
clean :
	rm -f $(OBJECTS) $(BIN) *.mod

