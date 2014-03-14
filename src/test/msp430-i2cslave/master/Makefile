##
# Make
##

TOPDIR=../../../..

include $(TOPDIR)/config/make/cross-mips.mak
include $(TOPDIR)/config/make/settings.mak

LDFLAGS = -lpthread -ldl

all: i2cblink i2cspeed

i2cblink: blink.c
	STAGING_DIR=$(STAGING_DIR) $(CC) $(CFLAGS) blink.c $(LDFLAGS) -o i2cblink

i2cspeed: speed.c
	STAGING_DIR=$(STAGING_DIR) $(CC) $(CFLAGS) speed.c $(LDFLAGS) -o i2cspeed

clean:
	@rm -f i2cblink
	@rm -f i2cspeed
