#!/bin/bash

MSPDEBUG=mspdebug
MSPDEBUG_IF=rf2500
BINARY=$1

if [ "X$BINARY" = "X" ]
then
	echo "No firmware file given"
	echo "usage: $0 <firmware.elf>"
	exit 1
fi

${MSPDEBUG} ${MSPDEBUG_IF} "prog ${BINARY}"
