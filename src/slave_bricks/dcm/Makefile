TOPDIR=../../..

include $(TOPDIR)/config/make/cross-msp430.mak
include $(TOPDIR)/config/make/settings.mak

# CFLAGS += -g

ifneq ($(I2C_ADDR),)
CFLAGS += -DI2C_ADDR=$(I2C_ADDR)
else
I2C_ADDR = 0x4C
endif

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
TARGET   = brick-o-lage_dcm_$(I2C_ADDR)_$(BINARY)


include $(TOPDIR)/config/make/rules-msp430.mak

