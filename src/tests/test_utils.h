#include "../constants/isa_constants.h"
#include "../constants/operation_constants.h"

#ifndef _TEST_UTILS_H
#define _TEST_UTILS_H


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

#endif
