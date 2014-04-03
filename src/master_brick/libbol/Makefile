##
# Make
##

TOPDIR=../../..

include $(TOPDIR)/config/make/cross-mips.mak
include $(TOPDIR)/config/make/settings.mak

CFLAGS 	+= -std=c++0x 
CFLAGS += -I./include
CFLAGS += -I$(TARGET_DIR)/usr/include 
CFLAGS += -I$(TARGET_DIR)/usr/include/python2.7

ARFLAGS  = rcs
SOURCES += brickbus.cpp
SOURCES += brickport.cpp 
SOURCES += brick.cpp 
SOURCES += diobrick.cpp
SOURCES += brickscript.cpp

OBJECTS = $(SOURCES:.cpp=.o)
LIBRARY = libbrick-o-lage.a

all: $(SOURCES) $(LIBRARY)
	
$(LIBRARY): $(OBJECTS)
	@#printf "  AR      $(subst $(shell pwd)/,,$(@))\n"
	$(AR) $(ARFLAGS) $@ $^

.cpp.o:
	STAGING_DIR=$(STAGING_DIR) $(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJECTS)
	@rm -f $(LIBRARY)
