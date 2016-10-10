#include "../e4c/e4c_lite.h"
#include <stdio.h>
#include "../stack.h"
#include "../types.h"
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

void stack_build_and_destroy(){
  fml_stack *stack = new_stack(5);
  destroy_stack(stack);
  CU_PASS("Did not crash when creating and destroying.");
}

void stack_standard_test(){
  fml_stack *stack = new_stack(5);
  fml_word input[5] = {0,1,2,3,4};
  fml_word output[5];

  E4C_TRY{
    for(int i = 0; i < 5; i++){
      push(stack,input[i]);
    }
  }E4C_CATCH(StackEmptyException){
    CU_FAIL_FATAL("StackEmpytException occured during sensible stack push.")
  }E4C_CATCH(StackOverflowException){
    CU_FAIL_FATAL("StackOverflowException occured during sensible stack push.")
  }

  E4C_TRY{
    for(int i = 4; i >= 0; i--){
      output[i] = pop(stack);
    }
  }E4C_CATCH(StackEmptyException){
    CU_FAIL("StackEmpytException occured during sensible stack pop.")
  }E4C_CATCH(StackOverflowException){
    CU_FAIL("StackOverflowException occured during sensible stack pop.")
  }

  for(int i = 0 ; i < 5; i++){
    if(input[i] != output[i]){
      CU_FAIL("Stack corrupted data."); 
    }
  }
  destroy_stack(stack);
}

void testEmptyException(){
  fml_stack *stack = new_stack(5);
  E4C_TRY{
    pop(stack); 
  }E4C_CATCH(StackEmptyException){
    //CU_FAIL("StackEmpytException occured during sensible stack pop.");
    CU_PASS();
  }E4C_CATCH(StackOverflowException){
    CU_FAIL("Stack overflow thrown whilst expecting Stack Empty");
  }
  destroy_stack(stack);
}

void testOverflowException(){
  fml_stack *stack = new_stack(5);
  E4C_TRY{
    int i;
    for(i = 0; i < 5; i++){
      push(stack,1);
    }
  }E4C_CATCH(StackEmptyException){
    CU_FAIL("Stack empty thrown when expecting stack overflow.");
  }E4C_CATCH(StackOverflowException){
    CU_ASSERT_EQUAL(stack->index,4);
    CU_PASS();
  }
  destroy_stack(stack);
}

