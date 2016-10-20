C_FLAGS = -Wall -std=c11 -ggdb
COMPILER = clang

test: e4c_lite.o eval_test.o stack_test.o stack.o ram.o ram_test.o tests/test_main.c fml_machine.o eval.o
	$(COMPILER) $(C_FLAGS)  -L/usr/local/lib  $^ -o ../bin/test -lcunit

eval_test.o: tests/eval_test.c eval.o fml_machine.o
	$(COMPILER) $(C_FLAGS) -c $^

eval.o: eval.c eval.h constants/binop_eval.h fml_machine.o e4c_lite.o 
	$(COMPILER) $(C_FLAGS) -c $^

fml_machine.o: fml_machine.c fml_machine.h ram.o stack.o e4c_lite.o
	$(COMPILER) $(C_FLAGS) -c $^

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

