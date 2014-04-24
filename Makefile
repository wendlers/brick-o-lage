#####################################################################
# Toplevel makefile for the Brick-o-Lage project 
#####################################################################

all: build

world: init build-carambola build

init: init-submodules init-carambola

build: build-msp430 build-bol

clean: clean-msp430 clean-bol

distclean: clean clean-carambola 

#####################################################################
# Prepare GIT submodules 
#####################################################################

init-submodules:
	@git submodule init
	@git submodule update	

#####################################################################
# Carambola (OpenWrt)
#####################################################################

init-carambola:	init-carambola-feeds init-carambola-config

init-carambola-feeds:
	@cd carambola && ./scripts/feeds update -a
	@cd carambola && ./scripts/feeds install -a

init-carambola-config:
	@cp config/carambola/openwrt.conf carambola/.config
	@make -C carambola oldconfig

build-carambola:
	@make -C carambola

clean-carambola: clean-carambola-test
	@make -C carambola distclean

#####################################################################
# MSP430
#####################################################################

build-msp430: build-msp430-libemb build-msp430-diofw

build-msp430-libemb:
	@make -C libemb TARCH=MSP430

build-msp430-diofw:
	@make -C src/slave_bricks/dio

clean-msp430: clean-msp430-libemb clean-msp430-diofw

clean-msp430-libemb:
	@make -C libemb TARCH=MSP430 clean

clean-msp430-diofw:
	@make -C src/slave_bricks/dio clean
	@rm -f bin/msp430/*.elf

#####################################################################
# Brick-o-Lage (on master) 
#####################################################################

build-bol: build-libbol build-libmongoosecpp build-bolsrv build-bolcmd 

build-libbol:
	@make -C src/master_brick/libbol

build-libmongoosecpp:
	@make -C src/master_brick/libmongoosecpp

build-bolsrv:
	@make -C src/master_brick/bolsrv

build-bolcmd:
	@make -C src/master_brick/bolcmd

clean-bol: clean-libbol clean-bolsrv clean-bolcmd clean-libmongoosecpp

clean-libbol:
	@make -C src/master_brick/libbol clean

clean-libmongoosecpp:
	@make -C src/master_brick/libmongoosecpp clean

clean-bolsrv:
	@make -C src/master_brick/bolsrv clean

clean-bolcmd:
	@make -C src/master_brick/bolcmd clean
