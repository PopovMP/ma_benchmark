OBJDIR = ./obj
SRCDIR = ./src
LIBDIR = ./lib

CC    = /usr/bin/gfortran
WARN  = -Wall -Wextra -Wpedantic
STD   = -std=f2008
OPT   = -O2

BIN   = ./main

OBJECTS = $(OBJDIR)/data_set.o       \
          $(OBJDIR)/shared_state.o   \
          $(OBJDIR)/moving_average.o \
          $(OBJDIR)/rsi.o

.PHONY : all
all : $(BIN)

$(BIN) : $(OBJECTS)
	$(CC) $(WARN) $(STD) $(OPT) -o $@ $^ $(SRCDIR)/$(BIN).f90

$(OBJDIR)/%.o : $(LIBDIR)/%.f90
	$(CC) $(WARN) $(STD) $(OPT) -c -o $@ $<

.PHONY : clean
clean :
	rm -f $(OBJECTS) $(BIN) *.mod

