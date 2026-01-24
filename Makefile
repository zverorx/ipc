CC ?= gcc

# Build mode: 'debug' or 'release'
BUILD ?= release

# Installation directory
PREFIX ?= /usr/local/bin/

TARGET = ipc
SRCDIR = src
INCDIR = include
OBJDIR = obj
BINDIR = bin

HEADERS = $(INCDIR)/ipv4_t.h	\
		  $(INCDIR)/fill_ipv4.h	\
		  $(INCDIR)/analysis.h	\
		  $(INCDIR)/subnet.h

SOURCES = $(SRCDIR)/main.c		\
		  $(SRCDIR)/fill_ipv4.c	\
		  $(SRCDIR)/analysis.c	\
		  $(SRCDIR)/subnet.c	

OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))

CFLAGS = -std=gnu99

CPPFLAGS = -I$(INCDIR)

ifeq ($(BUILD), debug)
	CFLAGS += -g -Wall
else
	ifeq ($(BUILD), release)
		CFLAGS += -O2
	endif
endif

all: $(TARGET) 

$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)/
	$(CC) $(CFLAGS) $^ -o $(BINDIR)/$@

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HEADERS)
	@mkdir -p $(OBJDIR)/
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

clean:
	rm -fr $(OBJDIR)/

distclean:
	rm -fr $(BINDIR)/ $(OBJDIR)/

install:
	install -d $(PREFIX)
	install -m 755 ./$(BINDIR)/$(TARGET) $(PREFIX)

uninstall:
	rm -f $(PREFIX)/$(TARGET)

help:
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

.PHONY: all clean distclean install uninstall help
