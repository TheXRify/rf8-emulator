RUN ?=

buildall:
	make -C ./emulator build
	make -C ./disassembler build
	make -C ./assembler build

run:
	make -C ./$(RUN) run