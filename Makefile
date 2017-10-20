CC = g++
MKDIR = mkdir -p
TARGET = main

SRCDIR = src
HDRDIR = include
BINDIR = bin
OBJDIR = obj
EXE := $(BINDIR)/$(TARGET)

SOURCES := $(wildcard $(SRCDIR)/*.cpp)
INCLUDES := $(wildcard $(HDRDIR)/*.h)
OBJECTS := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

CPPFLAGS += -I$(HDRDIR)
CFLAGS += -Wall

all: directories $(EXE)

$(EXE): $(OBJECTS)
	$(CC) $(CPPFLAGS) $(TARGET).cpp $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

directories:
	$(MKDIR) $(BINDIR) $(OBJDIR)

clean:
	rm -rf $(OBJDIR) $(BINDIR)
