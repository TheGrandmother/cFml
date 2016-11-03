#THIS IS A TEMPORARY MAKE FILE. IT  Will be replaced by a better one!!!
C_FLAGS = -Wall -std=c11 -ggdb
COMPILER = gcc

test: FORCE
	make -C src test
	./bin/test

constants: src/constants/operation_constants.h src/constants/isa_constants.h spec/generate_specs.py src/constants/binop_eval.h src/constants/reverse_lookup.h src/constants/reverse_lookup.h src/constants/memmory_constants.h

src/constants/operation_constants.h: spec/instructions.json spec/ISA.json spec/generate_specs.py
	cd spec/; python generate_specs.py -o -w
	mv spec/operation_constants.h $@

src/constants/isa_constants.h: spec/ISA.json spec/generate_specs.py
	cd spec/; python generate_specs.py -i -w
	mv spec/isa_constants.h $@

src/constants/binop_eval.h: spec/instructions.json spec/generate_specs.py
	cd spec/; python generate_specs.py -c -w
	mv spec/binop_eval.h $@

src/constants/reverse_lookup.h: spec/instructions.json spec/generate_specs.py
	cd spec/; python generate_specs.py -rl -w
	mv spec/reverse_lookup.h $@

src/constants/memmory_pages.h: spec/special_pages.json spec/generate_specs.py
	cd spec/; python generate_specs.py -pf -w
	mv spec/memmory_pages.h $@

src/constants/memmory_constants.h: spec/special_pages.json spec/generate_specs.py
	cd spec/; python generate_specs.py -mc -w
	mv spec/memmory_constants.h $@

FORCE:

