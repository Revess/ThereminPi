CC := g++
SRCDIR := src
BUILDDIR := build
TARGET := bin/thereminPi

SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -I/usr/local/include -Wall -std=c++1z
LDFLAGS := -ljack -lwiringPi
INC := -I include

$(TARGET): $(OBJECTS)
	@echo " Linking..."
	@echo " $(CC) $^ -o $(TARGET) $(LDFLAGS)"; $(CC) $^ -o $(TARGET) $(LDFLAGS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo " Cleaning...";
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)


tester:
	$(CC) $(CFLAGS) test/main.cpp $(INC) $(LDFLAGS) -o testFile


ticket:
	$(CC) $(CFLAGS) spikes/ticket.cpp $(INC) $(LDFLAGS) -o bin/ticket

.PHONY: clean
