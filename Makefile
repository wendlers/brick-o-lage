
init: init-submodules init-carambola

build: build-carambola build-msp430 build-bol

clean: clean-msp430 clean-bol

distclean: clean clean-carambola 

init-submodules:
	@git submodule init
	@git submodule update	

init-carambola:	init-carambola-feeds init-carambola-config

init-carambola-feeds:
	@cd carambola && ./scripts/feeds update -a
	@cd carambola && ./scripts/feeds install -a

init-carambola-config:
	@cp config/carambola/openwrt.conf carambola/.config
	@make -C carambola oldconfig

build-carambola:
	@make -C carambola

build-carambola-test:
	@make -C src/test/msp430-i2cslave/master 	

build-msp430: build-msp430-libemb build-msp430-diofw

build-msp430-libemb:
	@make -C libemb TARCH=MSP430

build-msp430-diofw:
	@make -C src/slave_bricks/dio

build-msp430-test:
	@make -C src/test/msp430-i2cslave/slave

build-bol: build-libbol build-libmongoosecpp build-bolsrv build-bolcmd 

build-libbol:
	@make -C src/master_brick/libbol

build-libmongoosecpp:
	@make -C src/master_brick/libmongoosecpp

build-bolsrv:
	@make -C src/master_brick/bolsrv

build-bolcmd:
	@make -C src/master_brick/bolcmd

clean-carambola: clean-carambola-test
	@make -C carambola distclean

clean-carambola-test:
	@make -C src/test/msp430-i2cslave/master clean

clean-msp430: clean-msp430-test clean-msp430-libemb clean-msp430-diofw

clean-msp430-libemb:
	@make -C libemb TARCH=MSP430 clean

clean-msp430-diofw:
	@make -C src/slave_bricks/dio clean

clean-msp430-test:
	@make -C src/test/msp430-i2cslave/slave clean

clean-bol: clean-libbol clean-bolsrv clean-bolcmd clean-libmongoosecpp

clean-libbol:
	@make -C src/master_brick/libbol clean

clean-libmongoosecpp:
	@make -C src/master_brick/libmongoosecpp clean

clean-bolsrv:
	@make -C src/master_brick/bolsrv clean

clean-bolcmd:
	@make -C src/master_brick/bolcmd clean


