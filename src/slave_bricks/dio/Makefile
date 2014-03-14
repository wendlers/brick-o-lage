TOPDIR=../../..

include $(TOPDIR)/config/make/cross-msp430.mak
include $(TOPDIR)/config/make/settings.mak

BINARY	 = firmware
OBJS	+= main.o cmd.o
INCDIR  += -I$(TOPDIR)/libemb/libi2c/src/include
INCDIR  += -I$(TOPDIR)/libemb/libserial/src/include
INCDIR  += -I$(TOPDIR)/libemb/libconio/src/include
LIBDIR  += -L$(TOPDIR)/libemb/libi2c/lib 
LIBDIR  += -L$(TOPDIR)/libemb/libserial/lib 
LIBDIR  += -L$(TOPDIR)/libemb/libconio/lib 
LIBS	+= -li2c -lserial -lconio
LIBS	+= -li2c 
TARGET   = brick-o-lage_dio_$(BINARY)

include $(TOPDIR)/config/make/rules-msp430.mak

flash:
	$(MSPDEBUG) $(MSPDEBUG_IF) "prog $(BINARY).elf"
