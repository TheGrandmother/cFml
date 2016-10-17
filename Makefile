#THIS IS A TEMPORARY MAKE FILE. IT  Will be replaced by a better one!!!
C_FLAGS = -Wall -std=c11 -ggdb
COMPILER = gcc

bin/test: FORCE
	make -C src test
	./bin/test

constants: src/constants/operation_constants.h src/constants/isa_constants.h spec/generate_specs.py

src/constants/operation_constants.h: spec/instructions.json spec/generate_specs.py
	cd spec/; python generate_specs.py -o -w
	mv spec/operation_constants.h $@


src/constants/isa_constants.h: spec/ISA.json spec/generate_specs.py
	cd spec/; python generate_specs.py -i -w
	mv spec/isa_constants.h $@

FORCE:

