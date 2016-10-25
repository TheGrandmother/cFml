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

#define NO_ARG 0

#define ADDR_REG_X (ADDRESS_MASK | REG_X) 
#define ADDR_REG_Y (ADDRESS_MASK | REG_Y) 
#define ADDR_CONSTANT (ADDRESS_MASK | CONSTANT) 
#define ADDR_STACK (ADDRESS_MASK | ACC_STACK) 
#define ADDR_SP (ADDRESS_MASK | SP) 

#define SP_REG_X (SP_MASK | REG_X) 
#define SP_REG_Y (SP_MASK | REG_Y) 
#define SP_CONSTANT (SP_MASK | CONSTANT) 
#define SP_STACK (SP_MASK | SP) 

#define INST_3(OP_CODE, STEP,A0,A1) (OP_CODE << OPCODE_SHIFT)\
  | (STEP << STEP_SHIFT)\
  | (A0 << A0_SHIFT)\
  | (A1 << A1_SHIFT)

#define INST_2(OP_CODE, STEP,A0) (OP_CODE << OPCODE_SHIFT)\
  | (STEP << STEP_SHIFT)\
  | (A0 << A0_SHIFT)

#define INST_1(OP_CODE) (OP_CODE << OPCODE_SHIFT)\
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
    INST_3(MOV_VALUE,2,CONSTANT,REG_X),
    TEST_DEFAULT,                                              // const
    (HLT_VALUE << OPCODE_SHIFT)                       // HLT
  };
  memcpy(fml->ram->prg_ram,program,sizeof(fml_word)*10);
  CU_ASSERT_EQUAL(eval(fml,2),0);
  CU_ASSERT_EQUAL(fml->x,TEST_DEFAULT);
  destroy_machine(fml);
}

void test_mov_const_sp(){
  fml_machine *fml = create_machine(100,10,100,100);
  fml_word program[10] ={
    INST_3(MOV_VALUE,2,CONSTANT,SP),
    TEST_DEFAULT,                                              // const
    (HLT_VALUE << OPCODE_SHIFT)                       // HLT
  };
  memcpy(fml->ram->prg_ram,program,sizeof(fml_word)*10);
  CU_ASSERT_EQUAL(eval(fml,2),0);
  CU_ASSERT_EQUAL(fml->sp,TEST_DEFAULT);
  destroy_machine(fml);
}

void test_mov_sp_x(){
  fml_machine *fml = create_machine(100,10,100,100);
  fml_word program[10] ={
    INST_3(MOV_VALUE,2,CONSTANT,SP),
    TEST_DEFAULT,
    INST_3(MOV_VALUE,1,SP,REG_X),
    (HLT_VALUE << OPCODE_SHIFT)                       // HLT
  };
  memcpy(fml->ram->prg_ram,program,sizeof(fml_word)*10);
  CU_ASSERT_EQUAL(eval(fml,20),0);
  CU_ASSERT_EQUAL(fml->x,TEST_DEFAULT);
  destroy_machine(fml);
}

void test_mov_addr_sp_x(){
  fml_machine *fml = create_machine(100,10,100,100);
  fml_word program[10] ={
    INST_3(MOV_VALUE,2, CONSTANT, SP),
    5,
    INST_3(MOV_VALUE,2,CONSTANT, ADDR_SP),
    420,
    INST_3(MOV_VALUE,1,ADDR_SP,REG_X),
    (HLT_VALUE << OPCODE_SHIFT)                       // HLT
  };
  memcpy(fml->ram->prg_ram,program,sizeof(fml_word)*10);
  CU_ASSERT_EQUAL(eval(fml,100),0);
  CU_ASSERT_EQUAL(fml->x,TEST_DEFAULT);
  destroy_machine(fml);
}

void test_mov_const_mem(){
  fml_machine *fml = create_machine(100,10,100,100);
  fml_word program[10] ={
    INST_3(MOV_VALUE,3,CONSTANT,ADDR_CONSTANT),
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
    INST_3(MOV_VALUE,2,CONSTANT,ACC_STACK),
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
    INST_3(ADD_VALUE,3,CONSTANT,CONSTANT),
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
    INST_3(MOV_VALUE,2,CONSTANT,REG_X),
    TEST_DEFAULT,                                              // const 
    INST_3(EQL_VALUE,2,CONSTANT,REG_X),
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
    INST_3(DIV_VALUE,3,CONSTANT,CONSTANT),
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
    INST_3(MOD_VALUE,3,CONSTANT,CONSTANT),
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
    INST_3(EQL_VALUE, 3, CONSTANT, CONSTANT),
    420,
    420,
    INST_3(JOO_VALUE, 2, CONSTANT, REG_X),
    6,
    INST_1(DIE_VALUE),
    INST_1(HLT_VALUE)

  };
  memcpy(fml->ram->prg_ram,program,sizeof(fml_word)*10);
  E4C_TRY{
    CU_ASSERT_EQUAL(eval(fml,1000),0);
  }E4C_CATCH(SuicideException){
    CU_FAIL("Program threw Suicide :'(\n");
  }
  destroy_machine(fml);
}

void test_joz(){
  fml_machine *fml = create_machine(100,10,100,100);
  fml_word program[10] ={
    INST_3(EQL_VALUE, 3, CONSTANT, CONSTANT),
    400,
    420,
    INST_3(JOZ_VALUE, 2, CONSTANT, REG_X),
    6,
    INST_1(DIE_VALUE),
    INST_1(HLT_VALUE)
  };
  memcpy(fml->ram->prg_ram,program,sizeof(fml_word)*10);
  E4C_TRY{
    CU_ASSERT_EQUAL(eval(fml,1000),0);
  }E4C_CATCH(SuicideException){
    CU_FAIL("Program threw Suicide :'(\n");
  }
  destroy_machine(fml);
}

void test_jmp(){
  fml_machine *fml = create_machine(100,10,100,100);
  fml_word program[10] ={
    INST_3(JMP_VALUE, 2, CONSTANT, REG_X),
    3,
    INST_1(DIE_VALUE),
    INST_1(HLT_VALUE)
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
    /*0*/  INST_3(MOV_VALUE, 2, CONSTANT, REG_X),
    /*1*/  400,
    /*2*/  INST_3(JSR_VALUE, 2, CONSTANT, REG_X),
    /*3*/  11,
    /*4*/  INST_3(ADD_VALUE, 1, REG_Y, REG_X),
    /*5*/  INST_3(EQL_VALUE, 2, ACC_STACK, CONSTANT),
    /*6*/  420,
    /*7*/  INST_3(JOO_VALUE, 2, CONSTANT, REG_X),
    /*8*/  10,
    /*9*/  INST_1(DIE_VALUE),
    /*10*/ INST_1(HLT_VALUE),
    /*11*/ INST_3(MOV_VALUE, 2, CONSTANT, REG_Y),
    /*12*/ 20,
    /*13*/ INST_1(RET_VALUE)

  };
  memcpy(fml->ram->prg_ram,program,sizeof(fml_word)*15);

  E4C_TRY{
    CU_ASSERT_EQUAL(eval(fml,1000),0);
  }E4C_CATCH(SuicideException){
    CU_FAIL("Program threw Suicide :'(\n");
  }
  destroy_machine(fml);
}

void test_nested_jsr(){
  fml_machine *fml = create_machine(100,15,100,100);
  fml_word program[15] ={
    /*0*/ INST_3(JSR_VALUE, 2, CONSTANT, NO_ARG),
    /*1*/ 7,
    /*2*/ INST_3(EQL_VALUE, 1, REG_X, REG_Y),
    /*3*/ INST_3(JOZ_VALUE, 2, CONSTANT, NO_ARG),
    /*4*/ 6,
    /*5*/ INST_1(HLT_VALUE),
    /*6*/ INST_1(DIE_VALUE),
    /*7*/ INST_3(INC_VALUE, 1, REG_Y, NO_ARG), //Bar
    /*8*/ INST_3(JSR_VALUE, 2, CONSTANT, NO_ARG),
    /*9*/ 11,
    /*10*/ INST_1(RET_VALUE),
    /*11*/ INST_3(INC_VALUE, 1,REG_X, NO_ARG), //foo
    /*12*/ INST_1(RET_VALUE)
  };
  memcpy(fml->ram->prg_ram,program,sizeof(fml_word)*15);

  E4C_TRY{
    CU_ASSERT_EQUAL(eval(fml,1000),0);
  }E4C_CATCH(SuicideException){
    CU_FAIL("Program threw Suicide :'(\n");
  }
  destroy_machine(fml);
}

void test_empty_jump_stack(){
  fml_machine *fml = create_machine(100,10,100,100);
  fml_word program[10] ={
    INST_1(RET_VALUE),
    INST_1(DIE_VALUE),
  };
  memcpy(fml->ram->prg_ram,program,sizeof(fml_word)*10);
  E4C_TRY{
    CU_ASSERT_EQUAL(eval(fml,1000),0);
  }E4C_CATCH(SuicideException){
    CU_FAIL("Program threw Suicide :'(\n");
  }E4C_CATCH(EmptyJumpStackException){
    CU_PASS();
  }
  destroy_machine(fml);
}

void test_soo(){
  fml_machine *fml = create_machine(100,15,100,100);
  fml_word program[15] ={
    INST_3(MOV_VALUE, 2, CONSTANT, ACC_STACK),
    1,
    INST_3(SOO_VALUE, 2, CONSTANT, NO_ARG),
    8,
    INST_3(JOO_VALUE, 2, CONSTANT, NO_ARG),
    7,
    INST_1(DIE_VALUE),
    INST_1(HLT_VALUE),
    INST_3(MOV_VALUE, 2, CONSTANT, ACC_STACK),
    1,
    INST_1(RET_VALUE)
  };
  memcpy(fml->ram->prg_ram,program,sizeof(fml_word)*15);
  E4C_TRY{
    CU_ASSERT_EQUAL(eval(fml,1000),0);
  }E4C_CATCH(SuicideException){
    CU_FAIL("Program threw Suicide :'(\n");
  }
  destroy_machine(fml);
}

void test_soz(){
  fml_machine *fml = create_machine(100,15,100,100);
  fml_word program[15] ={
    INST_3(MOV_VALUE, 2, CONSTANT, ACC_STACK),
    0,
    INST_3(SOZ_VALUE, 2, CONSTANT, NO_ARG),
    8,
    INST_3(JOO_VALUE, 2, CONSTANT, NO_ARG),
    7,
    INST_1(DIE_VALUE),
    INST_1(HLT_VALUE),
    INST_3(MOV_VALUE, 2, CONSTANT, ACC_STACK),
    1,
    INST_1(RET_VALUE)
  };
  memcpy(fml->ram->prg_ram,program,sizeof(fml_word)*15);
  E4C_TRY{
    CU_ASSERT_EQUAL(eval(fml,1000),0);
  }E4C_CATCH(SuicideException){
    CU_FAIL("Program threw Suicide :'(\n");
  }
  destroy_machine(fml);
}

void test_fib(){
  fml_machine *fml = create_machine(100,35,10,10);
  fml_word program[35] ={
    /*0*/ INST_3(MOV_VALUE, 2, CONSTANT, ACC_STACK),
    /*1*/ 10,
    /*2*/ INST_2(JSR_VALUE,2,CONSTANT),
    /*3*/ 5,
    /*4*/ INST_1(HLT_VALUE),

    /*5*/ INST_2(INC_VALUE,1,SP), //FIB BEGINS HERE
    /*6*/ INST_3(MOV_VALUE,1,ACC_STACK, ADDR_SP),
    /*7*/ INST_3(EQL_VALUE,2,CONSTANT,ADDR_SP),
    /*8*/ 0,
    /*9*/ INST_2(JOZ_VALUE,2,CONSTANT),
    /*10*/ 15,
    /*11*/ INST_3(MOV_VALUE,2,CONSTANT,ACC_STACK),
    /*12*/ 0,
    /*13*/ INST_2(DEC_VALUE,1,SP),
    /*14*/ INST_1(RET_VALUE),

    /*15*/ INST_3(EQL_VALUE,2,CONSTANT, ADDR_SP),// CASE_1
    /*16*/ 1,
    /*17*/ INST_2(JOZ_VALUE,2,CONSTANT),
    /*18*/ 23,
    /*19*/ INST_3(MOV_VALUE,2,CONSTANT,ACC_STACK),
    /*20*/ 1,
    /*21*/ INST_2(DEC_VALUE,1,SP),
    /*22*/ INST_1(RET_VALUE),

    /*23*/ INST_3(SUB_VALUE,2,ADDR_SP,CONSTANT), //CASE 2
    /*24*/ 2,
    /*25*/ INST_2(JSR_VALUE,2,CONSTANT),
    /*26*/ 5,
    /*27*/ INST_3(SUB_VALUE,2,ADDR_SP,CONSTANT),
    /*28*/ 1,
    /*29*/ INST_2(JSR_VALUE,2,CONSTANT),
    /*30*/ 5,
    /*31*/ INST_3(ADD_VALUE,1,ACC_STACK, ACC_STACK),
    /*32*/ INST_2(DEC_VALUE,1,SP),
    /*33*/ INST_1(RET_VALUE)
  };
  memcpy(fml->ram->prg_ram,program,sizeof(fml_word)*35);
  E4C_TRY{
    CU_ASSERT_EQUAL(eval(fml,5000),0);
    printf("Stack has %lu\n",peep(fml->s));
  }E4C_CATCH(SuicideException){
    CU_FAIL("Program threw Suicide :'(\n");
  }
  destroy_machine(fml);
}

