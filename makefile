# Compiler
CC = g++
# Output directory
OUTPUTDIR = bin
# Sources root directory
SRCDIR = src
NAUTYDIR=../
# Source files
SRCFILES = main.cpp $(wildcard $(SRCDIR)/anonalg/*.cpp) $(wildcard $(SRCDIR)/equivalence/*.cpp) $(wildcard $(SRCDIR)/graph/*.cpp) $(wildcard $(SRCDIR)/measures/*.cpp )
SRCFILES := $(patsubst $(SRCDIR)/%,%,$(SRCFILES))
# Where to find source files
vpath %.cpp $(SRCDIR)
# Headers directories
INCLUDES = -I $(SRCDIR) -I $(NAUTYDIR)
# Compiler flags
CFLAGS = 
# Colored gcc output. Colors are nice
CFLAGS += -fdiagnostics-color=always
# Linker flags
LDFLAGS =
# Libraries needed by the linker: -L /lib/dir -l name
LDLIBS = -L $(OUTPUTDIR)/experiments

# Generated object files:
OBJECTS=$(addprefix $(OUTPUTDIR)/, $(SRCFILES:.cpp=.o))

all: $(OUTPUTDIR) $(OUTPUTDIR)/anonet
.PHONY: all

$(OUTPUTDIR)/anonet: $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $(OBJECTS) $(NAUTYDIR)/traces.o $(NAUTYDIR)/nauty.a $(LDLIBS)


# Output directory
$(OUTPUTDIR):
	mkdir -p $(OUTPUTDIR)
	mkdir -p $(OUTPUTDIR)/anonalg $(OUTPUTDIR)/equivalence $(OUTPUTDIR)/graph $(OUTPUTDIR)/measures


# Object files (automatic generation of dependencies)
$(OUTPUTDIR)/%.o: %.cpp
	$(CC) -c $(CFLAGS) $(INCLUDES) -MD -MP $< -o $@
# Include generated dependency files
-include $(OBJECTS:.o=.d)

# Cleanup rules
.PHONY: clean cleanobjects cleanmain removedir
clean: cleanobjects cleanmain removedir

cleanobjects:
	rm -f $(OUTPUTDIR)/*.o $(OUTPUTDIR)/*.d

cleanmain:
	rm -f $(OUTPUTDIR)/anonet
	
removedir:
	rm -R $(OUTPUTDIR)

