#include "../e4c/e4c_lite.h"
#include <stdio.h>
#include "../stack.h"
#include "../types.h"
#include "test_utils.h"
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
//#include <unistd.h>

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
#include "../constants/memmory_constants.h"
#include "../constants/pages/io_page_constants.h"



void redirect_test(){
  //int i  = dup2(fileno(stdin), fileno(stdout));
  //fprintf(stderr, "dup returned: %d \n",i);
}



void test_println(){
  fml_machine *fml = create_machine(100,50,100,100);
  fml_word program[50] ={
    INST_3(MOV_VALUE, 3, CONSTANT, ADDR_CONSTANT),'L',50,
    INST_3(MOV_VALUE, 3, CONSTANT, ADDR_CONSTANT),'O',51,
    INST_3(MOV_VALUE, 3, CONSTANT, ADDR_CONSTANT),'L',52,
    INST_3(MOV_VALUE, 3, CONSTANT, ADDR_CONSTANT),' ',53,
    INST_3(MOV_VALUE, 3, CONSTANT, ADDR_CONSTANT),'F',54,
    INST_3(MOV_VALUE, 3, CONSTANT, ADDR_CONSTANT),'A',55,
    INST_3(MOV_VALUE, 3, CONSTANT, ADDR_CONSTANT),'I',56,
    INST_3(MOV_VALUE, 3, CONSTANT, ADDR_CONSTANT),'L',57,
    INST_3(MOV_VALUE, 3, CONSTANT, ADDR_CONSTANT),'\0',58,
    INST_3(MOV_VALUE, 2, CONSTANT, ACC_STACK), 50,
    INST_2(EFC_VALUE, 2, CONSTANT), (IO_PAGE_INDEX << PAGE_SHIFT) | PRINTLN,
    INST_1(HLT_VALUE)

  };
  memcpy(fml->ram->prg_ram,program,sizeof(fml_word)*50);

  E4C_TRY{
    CU_ASSERT_EQUAL(eval(fml,1000),0);
  }E4C_CATCH(SuicideException){
    CU_FAIL("Program threw Suicide :'(\n");
  }
  destroy_machine(fml);
}
