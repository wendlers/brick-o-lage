##
# Make
##

TOPDIR=../../..

include $(TOPDIR)/config/make/cross-mips.mak
include $(TOPDIR)/config/make/settings.mak

MONGDIR = $(TOPDIR)/mongoose-cpp

CFLAGS  = -I$(MONGDIR)
CFLAGS += -I$(TARGET_DIR)/usr/include 

ARFLAGS  = rcs

SOURCES += $(MONGDIR)/mongoose.c
SOURCES += $(MONGDIR)/mongoose/Controller.cpp
#SOURCES += $(MONGDIR)/mongoose/JsonController.cpp
#SOURCES += $(MONGDIR)/mongoose/JsonResponse.cpp
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
 
OBJECTS = $(SOURCES:.cpp=.o) $(SOURCES:.c=.o)
LIBRARY	= libmongoosecpp.a

all: $(SOURCES) $(LIBRARY) copy-includes
	
$(LIBRARY): $(OBJECTS)
	@#printf "  AR      $(subst $(shell pwd)/,,$(@))\n"
	$(AR) $(ARFLAGS) $@ $^

.cpp.o:
	STAGING_DIR=$(STAGING_DIR) $(CC) $(CFLAGS) -c $< -o $@

copy-includes:
	@mkdir -p include/mongoose
	@cp $(MONGDIR)/mongoose.h include/.
	@cp $(MONGDIR)/mongoose/*.h include/mongoose/.

clean:
	@rm -f $(MONGDIR)/*.o
	@rm -f $(MONGDIR)/mongoose/*.o
	@rm -f $(LIBRARY)
	@rm -fr include
