
init: init-submodules init-carambola

init-submodules:
	@git submodule init
	@git submodule update	

init-carambola:	init-carambola-feeds

init-carambola-feeds:
	@cd carambola && ./scripts/feeds update -a
	@cd carambola && ./scripts/feeds install -a

init-carambola-config:
	@cp config/carambola/openwrt.conf carambola/.config

build: build-carambola

build-carambola:
	make -C carambola
