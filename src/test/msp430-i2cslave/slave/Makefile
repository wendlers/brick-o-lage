##
# Make
##

TOPDIR=../../../..

include $(TOPDIR)/config/make/cross-msp430.mak
include $(TOPDIR)/config/make/settings.mak

all:	firmware

firmware:	main.c
	$(CC) $(CFLAGS) main.c TI_USCI_I2C_slave.c -o firmware

flash:	firmware
	$(MSPDEBUG) $(MSPDEBUG_IF) "prog firmware"

clean:
	@if [ -e firmware ]; then rm firmware; fi
