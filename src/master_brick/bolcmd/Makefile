##
# Make
##

TOPDIR=../../..

include $(TOPDIR)/config/make/cross-mips.mak
include $(TOPDIR)/config/make/settings.mak

CFLAGS 	+= -std=c++0x 
CFLAGS	+= -I$(TOPDIR)/src/master_brick/libbol/include
LDFLAGS += -L$(TOPDIR)/src/master_brick/libbol -L$(TARGET_DIR)/usr/lib
LIBS	+= -lbrick-o-lage

SOURCES=main.cpp

OBJECTS=$(SOURCES:.cpp=.o) 
EXECUTABLE=bolcmd

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	STAGING_DIR=$(STAGING_DIR) $(CXX) $(LDFLAGS) $(OBJECTS) -o $@ $(LIBS)

.cpp.o:
	STAGING_DIR=$(STAGING_DIR) $(CXX) $(CFLAGS) -c $< -o $@ 

clean:
	@rm -f $(OBJECTS)
	@rm -f $(EXECUTABLE)