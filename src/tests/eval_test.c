#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdbool.h>

#include "../e4c/e4c_lite.h"
#include "../ram.h"
#include "../types.h"
#include "../components.h"
#include "../eval.h"
#include "../stack.h"
#include "../fml_machine.h"
#include "../constants/isa_constants.h"
#include "../constants/operation_constants.h"

void machine_create_and_destroy(){
  fml_machine *fml_ = create_machine(100,100,100,100);
  destroy_machine(fml_);
}

void test_inc(){
  fml_machine *fml = create_machine(100,100,100,100);
  fml_word inc_instr = (INC_VALUE << OPCODE_SHIFT) | (1 << STEP_SHIFT); //INC s
  fml_word hlt_instruction = (HLT_VALUE << OPCODE_SHIFT); //INC x
  fml->ram->prg_ram[0] = inc_instr;
  fml->ram->prg_ram[1] = hlt_instruction;
  eval(fml,2);
  CU_ASSERT_EQUAL(fml->x, 1);
  //TODO: Test all locations.

  destroy_machine(fml);
}

void test_mov_const_x(){
  fml_machine *fml = create_machine(100,100,100,100);
  fml_word program[10] ={
    (MOV_VALUE << OPCODE_SHIFT) | (2 << STEP_SHIFT) |
      CONSTANT << A0_SHIFT | REG_X << A1_SHIFT,       // MOV 420 x
    420,                                              // const
    (HLT_VALUE << OPCODE_SHIFT)                       // HLT
  };
  memcpy(fml->ram->prg_ram,program,sizeof(fml_word)*10);
  CU_ASSERT_EQUAL(eval(fml,2),0);
  destroy_machine(fml);
}

void test_mov_const_mem(){
  fml_machine *fml = create_machine(100,100,100,100);
  fml_word program[10] ={
    (MOV_VALUE << OPCODE_SHIFT) | (3 << STEP_SHIFT) |
      (CONSTANT << A0_SHIFT) | ((ADDRESS_MASK | CONSTANT) << A1_SHIFT)
      ,       // MOV 420 $42
    420,                                              // const 
    42,                                               // const
    (HLT_VALUE << OPCODE_SHIFT)                       // HLT
  };
  memcpy(fml->ram->prg_ram,program,sizeof(fml_word)*10);

  CU_ASSERT_EQUAL(eval(fml,2),0);
  printf("Ram at $42 is 0x%LX(%lu)\n",fml->ram->ram[42],fml->ram->ram[42]);
  CU_ASSERT_EQUAL(fml->ram->ram[42], 420);

  destroy_machine(fml);
}

void test_mov_const_s(){
  fml_machine *fml = create_machine(100,100,100,100);
  fml_word program[10] ={
    (MOV_VALUE << OPCODE_SHIFT) | (2 << STEP_SHIFT) |
      (CONSTANT << A0_SHIFT) | ( ACC_STACK << A1_SHIFT)
      ,       // MOV 420 s
    420,                                              // const 
    (HLT_VALUE << OPCODE_SHIFT)                       // HLT
  };
  memcpy(fml->ram->prg_ram,program,sizeof(fml_word)*10);

  CU_ASSERT_EQUAL(eval(fml,2),0);
  //printf("Stack is: %LX\n",peep(fml->s));
  //print_stack(fml->s);
  CU_ASSERT_EQUAL(peep(fml->s), 420);

  destroy_machine(fml);
}

