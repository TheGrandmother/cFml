/**
 *
 * This file contains the stack implementation.
 *
 **/
#include "components.h"
#include "fail.h"

fml_stack *new_stack(size_t size){
  fml_stack *stack = calloc(size, sizeof(fml_stack));
  stack->size = size;
  stack->index = 0;
  stack->array = calloc(size, sizeof(fml_word));
  return stack;
}

void push(fml_stack *stack, fml_word data){
  if(stack->index != stack->size -1){
    stack->index++;
    stack->array[stack->index] = data;
  }else{
    crashAndBurn("Stack overflow");
  }
}

fml_word pop(fml_stack *stack){
  if(stack->index != 0){
    fml_word ret  = stack->array[stack->index];
    stack->index--;
    return ret;
  }else{
    crashAndBurn("Stack ovrflow");
    return 0; //Silence warning
  }
}

fml_word peep(fml_stack *stack){
  return stack->array[stack->index];
}

void destroy_stack(fml_stack *stack){
  free(stack->array);
  free(stack);
}
