
init: init-submodules init-carambola

build: build-carambola build-msp430

clean: clean-carambola clean-msp430

init-submodules:
	@git submodule init
	@git submodule update	

init-carambola:	init-carambola-feeds

init-carambola-feeds:
	@cd carambola && ./scripts/feeds update -a
	@cd carambola && ./scripts/feeds install -a

init-carambola-config:
	@cp config/carambola/openwrt.conf carambola/.config

build-carambola:
	@make -C carambola

build-carambola-test:
	@make -C src/test/msp430-i2cslave/master 	

build-msp430: build-msp430-test

build-msp430-libemb:
	@make -C libemb TARCH=MSP430

build-msp430-test:
	@make -C src/test/msp430-i2cslave/slave

clean-carambola: clean-carambola-test
	@make -C carambola distclean

clean-carambola-test:
	@make -C src/test/msp430-i2cslave/master clean

clean-msp430: clean-msp430-test

clean-msp430-test:
	@make -C src/test/msp430-i2cslave/slave clean
