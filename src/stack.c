/**
 *
 * This file contains the stack implementation.
 *
 **/
#include "components.h"
#include "fail.h"
#include "e4c/e4c_lite.h"
#include <stdio.h>

E4C_DEFINE_EXCEPTION(StackException, "Stack Exception.", RuntimeException);
E4C_DEFINE_EXCEPTION(StackEmptyException, "Stack empty.", StackException);
E4C_DEFINE_EXCEPTION(StackOverflowException, "Stack overflow.", StackException);


fml_stack *new_stack(size_t size){
  fml_stack *stack = malloc(size * sizeof(fml_stack));
  stack->size = size;
  stack->index = 0;
  stack->array = calloc(size, sizeof(fml_word));
  return stack;
}

void push(fml_stack *stack, fml_word data){
  if(stack->index < stack->size){
    stack->array[stack->index] = data;
    stack->index = 
      (stack->index == stack->size - 1) ? stack->index : stack->index + 1;
  }else{
    E4C_THROW(StackOverflowException,NULL);
  }
}

fml_word pop(fml_stack *stack){
  if(stack->index >= 0){
    fml_word ret  = stack->array[stack->index];
    stack->index = (stack->index == 0) ? 0 : stack->index - 1;
    return ret;
  }else{
    E4C_THROW(StackEmptyException,NULL);
    return 0; //Silence warning
  }

}

void print_stack(fml_stack *stack){
  for(int i = 0; i < stack->size; i++){
    printf("%lu ", stack->array[i]); 
  }
  printf("\nhead at %lu of size %lu\n",stack->index, stack->size);
}

fml_word peep(fml_stack *stack){
  return stack->array[stack->index];
}

void destroy_stack(fml_stack *stack){
  free(stack->array);
  free(stack);
}
