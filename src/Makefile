C_FLAGS = -Wall -std=c11 -ggdb
COMPILER = gcc

test: e4c_lite.o stack_test.o stack.o ram.o ram_test.o tests/test_main.c
	$(COMPILER) $(C_FLAGS)  -L/usr/local/lib  $^ -o ../bin/test -lcunit

stack_test.o: tests/stack_test.c tests/stack_test.h stack.o
	$(COMPILER) $(C_FLAGS) -c $^

ram_test.o: tests/ram_test.c tests/ram_test.h ram.o
	$(COMPILER) $(C_FLAGS) -c $^

stack.o: stack.c stack.h components.h e4c_lite.o
	$(COMPILER) $(C_FLAGS) -c $^

ram.o: ram.c ram.h components.h constants.h types.h e4c_lite.o
	$(COMPILER) $(C_FLAGS) -c $^

#This produces an error that i simply just does not understand :p
e4c_lite.o: e4c/e4c_lite.c  e4c/e4c_lite.h
	$(COMPILER) $(C_FLAGS)  -c $^

