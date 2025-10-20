# Compiler to use (default: gcc)
CC ?= gcc

# Build mode: 'debug' or 'release' (default: release)
BUILD ?= release

# Installation directory (default: /usr/local/bin/)
PREFIX ?= /usr/local/bin/

# Name of the final executable
TARGET = ipc

# Source directory
SRCDIR = src

# Include directory
INCDIR = include

# Objects directory
OBJDIR = obj

# Binary directory
BINDIR = bin

# List of header files
HEADERS = $(INCDIR)/ipc.h $(INCDIR)/macros.h

# List of source files
SOURCES = $(SRCDIR)/main.c $(SRCDIR)/ipc.c

# Object files generated from source files
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))

# Compiler flags
CFLAGS = -Wall -std=c99

# Preprocessor flags
CPPFLAGS = -I$(INCDIR)

# Add debug or optimization flags based on build mode
ifeq ($(BUILD), debug)
	CFLAGS += -g
else
	ifeq ($(BUILD), release)
		CFLAGS += -O2
	endif
endif

# Default target: build the program
all: $(TARGET) 

# Link object files into the final executable
$(TARGET): $(OBJECTS)
	mkdir -p $(BINDIR)/
	$(CC) $(CFLAGS) $^ -o $(BINDIR)/$@

# Compile .c files into .o files
$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HEADERS)
	mkdir -p $(OBJDIR)/
	@echo "Compiling $<..."
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

# Remove object files
clean:
	rm -fr $(OBJDIR)/

# Remove all generated files (objects and executable)
distclean:
	rm -fr $(BINDIR)/ $(OBJDIR)/

# Install the executable to the target directory
install:
	install -d $(PREFIX)
	install -m 755 ./$(BINDIR)/$(TARGET) $(PREFIX)

# Remove the installed executable
uninstall:
	rm -f $(PREFIX)/$(TARGET)

# Show help message
help:
	@echo "Hints:"
	@echo ""
	@echo "make - Build the program"
	@echo "make BUILD=debug - Build with debug flags"
	@echo "make BUILD=release - Build with optimization"
	@echo ""
	@echo "make clean - Remove all temporary files"
	@echo "make distclean - Remove all generated files"
	@echo "make install - Install the executable file to $(PREFIX)"
	@echo "make uninstall - Remove the installed executable file"
	@echo ""
	@echo "To change the installation and removal path, use PREFIX="

# Declare phony targets (not real files)
.PHONY: all clean distclean install uninstall help