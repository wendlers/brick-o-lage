CC 			= $(CROSS_COMPILE)gcc
CXX 		= $(CROSS_COMPILE)g++
LD			= $(CROSS_COMPILE)gcc
AR			= $(CROSS_COMPILE)ar
OBJCOPY		= $(CROSS_COMPILE)objcopy
OBJDUMP		= $(CROSS_COMPILE)objdump
STRIP		= $(CROSS_COMPILE)strip

CFLAGS += -Wall -Werror
