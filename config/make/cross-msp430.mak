# msp430-gcc PATH
CROSS_DIR = /usr/bin/

# Set arch to MSP430 
ARCH = msp430
MCU = g2553

# Set corss-compile prefix to mips-gcc
CROSS_COMPILE = $(CROSS_DIR)msp430-

# MSP debug
MSPDEBUG = $(CROSS_DIR)mspdebug
MSPDEBUG_IF = rf2500

CFLAGS += -mmcu=$(ARCH)$(MCU)
