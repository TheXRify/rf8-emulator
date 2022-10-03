RUN ?=

buildall:
	make -C ./emulator build
	make -C ./disassembler build

run:
	make -C ./$(RUN) run