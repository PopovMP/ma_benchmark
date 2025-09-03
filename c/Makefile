OBJDIR = ./obj
SRCDIR = ./src
LIBDIR = ./lib

CC   = /usr/bin/gcc
WARN = -Wall -Wextra -pedantic
LIBS = -lm
STD  = -std=c2x
ARCH = -march=x86-64-v3
OPT  = -Ofast  -ffast-math
BIN  = ./main

OBJECTS = $(OBJDIR)/data-set.o       \
          $(OBJDIR)/shared-state.o   \
          $(OBJDIR)/moving-average.o \
          $(OBJDIR)/rsi.o            \
          $(OBJDIR)/main.o           \
          $(OBJDIR)/std-dev.o

# Default target
.PHONY : all
all : $(OBJDIR) $(BIN)

# Create the object directory if it does not exist
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Compile the binary from the object files
$(BIN) : $(OBJECTS)
	$(CC) $(WARN) $(STD) $(ARCH) $(OPT) -o $@ $^ $(LIBS)
	strip $@

# Compile the object files
$(OBJDIR)/%.o : $(LIBDIR)/%.c $(LIBDIR)/%.h
	$(CC) $(WARN) $(STD) $(ARCH) $(OPT) -c -o $@ $<

# Comompile the main object file
$(OBJDIR)/main.o : $(SRCDIR)/main.c
	$(CC) $(WARN) $(STD) $(ARCH) $(OPT) -c -o $@ $<

# Clean up
.PHONY : clean
clean :
	rm -f $(OBJECTS) $(BIN)

# Compile main  and the libs to assembly
.PHONY : asm
asm : $(SRCDIR)/main.c $(LIBDIR)/data-set.c $(LIBDIR)/shared-state.c $(LIBDIR)/moving-average.c $(LIBDIR)/rsi.c
	$(CC) $(WARN) $(STD) $(ARCH) $(OPT) -S -Ofast -o $(SRCDIR)/main.asm $(SRCDIR)/main.c
	$(CC) $(WARN) $(STD) $(ARCH) $(OPT) -S -Ofast -o $(LIBDIR)/data-set.asm $(LIBDIR)/data-set.c
	$(CC) $(WARN) $(STD) $(ARCH) $(OPT) -S -Ofast -o $(LIBDIR)/shared-state.asm $(LIBDIR)/shared-state.c
	$(CC) $(WARN) $(STD) $(ARCH) $(OPT) -S -Ofast -o $(LIBDIR)/moving-average.asm $(LIBDIR)/moving-average.c
	$(CC) $(WARN) $(STD) $(ARCH) $(OPT) -S -Ofast -o $(LIBDIR)/rsi.asm $(LIBDIR)/rsi.c
	$(CC) $(WARN) $(STD) $(ARCH) $(OPT) -S -Ofast -o $(LIBDIR)/std-dev.asm $(LIBDIR)/std-dev.c

.PHONY : clean_asm
clean_asm :
	rm -f $(SRCDIR)/main.asm $(LIBDIR)/data-set.asm $(LIBDIR)/shared-state.asm $(LIBDIR)/moving-average.asm $(LIBDIR)/rsi.asm
