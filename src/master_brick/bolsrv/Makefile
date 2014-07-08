##
# Make
##

TOPDIR=../../..

include $(TOPDIR)/config/make/cross-mips.mak
include $(TOPDIR)/config/make/settings.mak

CFLAGS  = -Wall 
CFLAGS 	+= -std=c++0x 
CFLAGS += -I./include
CFLAGS	+= -I$(TOPDIR)/src/master_brick/libbol/include 
CFLAGS += -I$(TOPDIR)/src/master_brick/libbolchai/include 
CFLAGS += -I$(TOPDIR)/src/master_brick/libmongoosecpp/include
CFLAGS += -I$(TARGET_DIR)/usr/include 
CFLAGS += -I$(TARGET_DIR)/usr/include/python2.7

LDFLAGS += -L$(TOPDIR)/src/master_brick/libbol 
LDFLAGS += -L$(TOPDIR)/src/master_brick/libbolchai 
LDFLAGS += -L$(TOPDIR)/src/master_brick/libmongoosecpp
LDFLAGS += -L$(TARGET_DIR)/usr/lib

LIBS	+= -lbrick-o-lage
LIBS	+= -lmongoosecpp
LIBS	+= -lboost_thread
LIBS	+= -lboost_chrono
LIBS	+= -lboost_system
LIBS	+= -lboost_signals
LIBS	+= -lboost_program_options
LIBS	+= -lboost_python
LIBS	+= -lpthread
LIBS	+= -lpython2.7
LIBS	+= -lutil
LIBS	+= -ldl
LIBS	+= -lz
LIBS	+= -lcurl

SOURCES  = main.cpp
SOURCES += daemon.cpp
SOURCES += bolcontroller.cpp

OBJECTS 	= $(SOURCES:.cpp=.o) 
EXECUTABLE 	= bolsrv

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	STAGING_DIR=$(STAGING_DIR) $(CXX) $(LDFLAGS) $(OBJECTS) -o $@ $(LIBS)

.cpp.o:
	STAGING_DIR=$(STAGING_DIR) $(CXX) $(CFLAGS) -c $< -o $@ 

strip:
	$(STRIP) $(EXECUTABLE) 

clean:
	@rm -f *.o
	@rm -f $(EXECUTABLE)
