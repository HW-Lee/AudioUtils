CC = g++
MKDIR = mkdir -p
EXE = main

SRCDIR = src
HDRDIR = include
BINDIR = bin
OBJDIR = obj

SOURCES := $(wildcard $(SRCDIR)/*.cpp)
INCLUDES := $(wildcard $(HDRDIR)/*.h)
OBJECTS := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

CPPFLAGS += -I$(HDRDIR)
CFLAGS += -Wall

all: directories $(EXE)

$(EXE): $(OBJECTS)
	$(CC) $(CPPFLAGS) main.cpp $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

directories:
	$(MKDIR) $(BINDIR) $(OBJDIR)

clean:
	rm -rf $(OBJDIR)/*
