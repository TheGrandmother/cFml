#This is such a stub
#gcc -Wall -L/usr/local/lib -o stack_test fail.c stack.c e4c/e4c_lite.c tests/stack_test.c  tests/test_main.c -lcunit -ggdb
C_FLAGS = -Wall -std=c11 -ggdb
COMPILER = gcc

tests: src/fail.c src/stack.c src/e4c/e4c_lite.c src/tests/stack_test.c src/tests/test_main.c
	$(COMPILER) $(C_FLAGS)  -L/usr/local/lib  $^ -o $@ -lcunit
