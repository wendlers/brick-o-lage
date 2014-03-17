##
# Make
##

TOPDIR=../../..

include $(TOPDIR)/config/make/cross-mips.mak
include $(TOPDIR)/config/make/settings.mak

CFLAGS 	+= -std=c++0x -I./include 
ARFLAGS  = rcs

SOURCES=brickbus.cpp brickport.cpp brick.cpp diobrick.cpp 

OBJECTS=$(SOURCES:.cpp=.o)
LIBRARY=libbrick-o-lage.a

all: $(SOURCES) $(LIBRARY)
	
$(LIBRARY): $(OBJECTS)
	@#printf "  AR      $(subst $(shell pwd)/,,$(@))\n"
	$(AR) $(ARFLAGS) $@ $^

.cpp.o:
	STAGING_DIR=$(STAGING_DIR) $(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJECTS)
	@rm -f $(LIBRARY)
	@rm -fr $(LIBDIR)
