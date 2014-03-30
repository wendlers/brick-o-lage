##
# Make
##

TOPDIR=../../..

include $(TOPDIR)/config/make/cross-mips.mak
include $(TOPDIR)/config/make/settings.mak

MONGDIR = $(TOPDIR)/mongoose-cpp

CFLAGS  = -Wall 
CFLAGS 	+= -std=c++0x 
CFLAGS	+= -I$(TOPDIR)/src/master_brick/libbol/include 
CFLAGS += -I$(TOPDIR)/src/master_brick/libbolchai/include 
CFLAGS += -I$(MONGDIR)
CFLAGS += -I$(TARGET_DIR)/usr/include 
CFLAGS += -I$(TARGET_DIR)/usr/include/python2.7

LDFLAGS += -L$(TOPDIR)/src/master_brick/libbol 
LDFLAGS += -L$(TOPDIR)/src/master_brick/libbolchai 
LDFLAGS += -L$(TARGET_DIR)/usr/lib

LIBS	+= -lbrick-o-lage
LIBS	+= -lboost_thread
LIBS	+= -lboost_chrono
LIBS	+= -lboost_system
LIBS	+= -lboost_signals
LIBS	+= -lboost_python
LIBS	+= -lpthread
LIBS	+= -lpython2.7
LIBS	+= -lutil
LIBS	+= -ldl
LIBS	+= -lz

SOURCES  = main.cpp
SOURCES += $(MONGDIR)/mongoose.c
SOURCES += $(MONGDIR)/mongoose/Controller.cpp
# SOURCES += $(MONGDIR)/mongoose/JsonController.cpp
# SOURCES += $(MONGDIR)/mongoose/JsonResponse.cpp
SOURCES += $(MONGDIR)/mongoose/Mutex.cpp
SOURCES += $(MONGDIR)/mongoose/Request.cpp
SOURCES += $(MONGDIR)/mongoose/Response.cpp
SOURCES += $(MONGDIR)/mongoose/Server.cpp
SOURCES += $(MONGDIR)/mongoose/Session.cpp
SOURCES += $(MONGDIR)/mongoose/Sessions.cpp
SOURCES += $(MONGDIR)/mongoose/StreamResponse.cpp
SOURCES += $(MONGDIR)/mongoose/UploadFile.cpp
SOURCES += $(MONGDIR)/mongoose/Utils.cpp
SOURCES += $(MONGDIR)/mongoose/WebController.cpp
SOURCES += $(MONGDIR)/mongoose/WebSocket.cpp
SOURCES += $(MONGDIR)/mongoose/WebSockets.cpp
 
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
