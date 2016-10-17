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
#include "../fml_machine.h"
#include "../constants/isa_constants.h"
#include "../constants/operation_constants.h"

void machine_create_and_destroy(){
  fml_machine *fml = create_machine(100,100,100,100);
  destroy_machine(fml);
}

void test_inc(){
  fml_machine *fml = create_machine(100,100,100,100);
  fml_word inc_instr = (INC_VALUE << OPCODE_SHIFT) | (1 << STEP_SHIFT); //INC s
  fml_word hlt_instruction = (HLT_VALUE << OPCODE_SHIFT); //INC x
  fml->ram->prg_ram[0] = inc_instr;
  fml->ram->prg_ram[1] = hlt_instruction;
  eval(fml);
  CU_ASSERT_EQUAL(fml->x, 1);


}
