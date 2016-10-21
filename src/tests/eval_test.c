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
#include "../exceptions.h"
#include "../constants/isa_constants.h"
#include "../constants/operation_constants.h"

void machine_create_and_destroy(){
  fml_machine *fml_ = create_machine(100,100,100,100);
  destroy_machine(fml_);
}

#define TEST_DEFAULT 420

#define ADDR_REG_X (ADDRESS_MASK | REG_X) 
#define ADDR_REG_Y (ADDRESS_MASK | REG_Y) 
#define ADDR_CONSTANT (ADDRESS_MASK | CONSTANT) 
#define ADDR_STACK (ADDRESS_MASK | ACC_STACK) 

#define SP_REG_X (SP_MASK | REG_X) 
#define SP_REG_Y (SP_MASK | REG_Y) 
#define SP_CONSTANT (SP_MASK | CONSTANT) 
#define SP_STACK (SP_MASK | ACC_STACK) 

#define INSTRUCTION(OP_CODE, STEP,A0,A1) (OP_CODE << OPCODE_SHIFT)\
  | (STEP << STEP_SHIFT)\
  | (A0 << A0_SHIFT)\
  | (A1 << A1_SHIFT)

#define INSTRUCTION_(OP_CODE) (OP_CODE << OPCODE_SHIFT)\
  | (1 << STEP_SHIFT)


void test_inc(){
  fml_machine *fml = create_machine(100,10,100,100);
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
  fml_machine *fml = create_machine(100,10,100,100);
  fml_word program[10] ={
    INSTRUCTION(MOV_VALUE,2,CONSTANT,REG_X),
    TEST_DEFAULT,                                              // const
    (HLT_VALUE << OPCODE_SHIFT)                       // HLT
  };
  memcpy(fml->ram->prg_ram,program,sizeof(fml_word)*10);
  CU_ASSERT_EQUAL(eval(fml,2),0);
  destroy_machine(fml);
}

void test_mov_const_mem(){
  fml_machine *fml = create_machine(100,10,100,100);
  fml_word program[10] ={
    INSTRUCTION(MOV_VALUE,3,CONSTANT,ADDR_CONSTANT),
    TEST_DEFAULT,                                              // const 
    42,                                               // const
    (HLT_VALUE << OPCODE_SHIFT)                       // HLT
  };
  memcpy(fml->ram->prg_ram,program,sizeof(fml_word)*10);

  CU_ASSERT_EQUAL(eval(fml,2),0);
  printf("Ram at $42 is 0x%lX(%lu)\n",fml->ram->ram[42],fml->ram->ram[42]);
  CU_ASSERT_EQUAL(fml->ram->ram[42], TEST_DEFAULT);

  destroy_machine(fml);
}

void test_mov_const_s(){
  fml_machine *fml = create_machine(100,10,100,100);
  fml_word program[10] ={
    INSTRUCTION(MOV_VALUE,2,CONSTANT,ACC_STACK),
    TEST_DEFAULT,                                              // const 
    (HLT_VALUE << OPCODE_SHIFT)                       // HLT
  };
  memcpy(fml->ram->prg_ram,program,sizeof(fml_word)*10);

  CU_ASSERT_EQUAL(eval(fml,2),0);
  CU_ASSERT_EQUAL(peep(fml->s), TEST_DEFAULT);

  destroy_machine(fml);
}

void test_add_c_c(){
  fml_machine *fml = create_machine(100,10,100,100);
  fml_word program[10] ={
    INSTRUCTION(ADD_VALUE,3,CONSTANT,CONSTANT),
    20,                                              // const 
    400,                                              // const 
    (HLT_VALUE << OPCODE_SHIFT)                       // HLT
  };
  memcpy(fml->ram->prg_ram,program,sizeof(fml_word)*10);

  CU_ASSERT_EQUAL(eval(fml,2),0);
  CU_ASSERT_EQUAL(peep(fml->s), TEST_DEFAULT);

  destroy_machine(fml);
}

void test_eql_x_c(){
  fml_machine *fml = create_machine(100,10,100,100);
  fml_word program[10] ={
    INSTRUCTION(MOV_VALUE,2,CONSTANT,REG_X),
    TEST_DEFAULT,                                              // const 
    INSTRUCTION(EQL_VALUE,2,CONSTANT,REG_X),
    TEST_DEFAULT,
    (HLT_VALUE << OPCODE_SHIFT)                       // HLT
  };
  memcpy(fml->ram->prg_ram,program,sizeof(fml_word)*10);

  CU_ASSERT_EQUAL(eval(fml,5),0);
  CU_ASSERT_EQUAL(peep(fml->s), 1);

  destroy_machine(fml);
}


void test_div_420_0_fail(){
  fml_machine *fml = create_machine(100,10,100,100);

  fml_word program[10] ={
    INSTRUCTION(DIV_VALUE,3,CONSTANT,CONSTANT),
    420,                                              // const 
    0,                                              // const 
    (HLT_VALUE << OPCODE_SHIFT)                       // HLT
  };
  memcpy(fml->ram->prg_ram,program,sizeof(fml_word)*10);

  E4C_TRY{
    eval(fml,2);
  }E4C_CATCH(RuntimeException){
    CU_PASS();
    return;
  }
  printf("Did not throw exception on div by 0\n");
  CU_FAIL();
  destroy_machine(fml);
}

void test_mod_420_0_fail(){
  fml_machine *fml = create_machine(100,10,100,100);

  fml_word program[10] ={
    INSTRUCTION(MOD_VALUE,3,CONSTANT,CONSTANT),
    420,                                              // const 
    0,                                                // const 
    (HLT_VALUE << OPCODE_SHIFT)                       // HLT
  };
  memcpy(fml->ram->prg_ram,program,sizeof(fml_word)*10);

  E4C_TRY{
    eval(fml,2);
  }E4C_CATCH(ModByZeroException){
    CU_PASS();
    return;
  }E4C_CATCH(RuntimeException){
    printf("Did not catch correct exception.");
    CU_FAIL();
  }
  printf("Did not throw exception on mod by 0\n");
  CU_FAIL();
  destroy_machine(fml);
}

void test_joo(){
  fml_machine *fml = create_machine(100,10,100,100);
  fml_word program[10] ={
    INSTRUCTION(EQL_VALUE, 3, CONSTANT, CONSTANT),
    420,
    420,
    INSTRUCTION(JOO_VALUE, 2, CONSTANT, REG_X),
    6,
    INSTRUCTION_(DIE_VALUE),
    INSTRUCTION_(HLT_VALUE)

  };
  memcpy(fml->ram->prg_ram,program,sizeof(fml_word)*10);

  E4C_TRY{
    CU_ASSERT_EQUAL(eval(fml,1000),0);
  }E4C_CATCH(SuicideException){
    CU_FAIL("Program threw Suicide :'(\n");
  }

  destroy_machine(fml);
}
void test_jsr(){
  fml_machine *fml = create_machine(100,15,100,100);
  fml_word program[15] ={
    /*0*/  INSTRUCTION(MOV_VALUE, 2, CONSTANT, REG_X),
    /*1*/  400,
    /*2*/  INSTRUCTION(JSR_VALUE, 2, CONSTANT, REG_X),
    /*3*/  11,
    /*4*/  INSTRUCTION(ADD_VALUE, 1, REG_Y, REG_X),
    /*5*/  INSTRUCTION(EQL_VALUE, 2, ACC_STACK, CONSTANT),
    /*6*/  420,
    /*7*/  INSTRUCTION(JOO_VALUE, 2, CONSTANT, REG_X),
    /*8*/  10,
    /*9*/  INSTRUCTION_(DIE_VALUE),
    /*10*/ INSTRUCTION_(HLT_VALUE),
    /*11*/ INSTRUCTION(MOV_VALUE, 2, CONSTANT, REG_Y),
    /*12*/ 20,
    /*13*/ INSTRUCTION_(RET_VALUE)

  };
  memcpy(fml->ram->prg_ram,program,sizeof(fml_word)*15);

  E4C_TRY{
    CU_ASSERT_EQUAL(eval(fml,1000),0);
  }E4C_CATCH(SuicideException){
    CU_FAIL("Program threw Suicide :'(\n");
  }

  destroy_machine(fml);
}

