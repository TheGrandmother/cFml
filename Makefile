#THIS IS A TEMPORARY MAKE FILE. IT  Will be replaced by a better one!!!
C_FLAGS = -Wall -std=c11 -ggdb
COMPILER = gcc

bin/test: 
	make -C src test
	./bin/test
	rm bin/test

