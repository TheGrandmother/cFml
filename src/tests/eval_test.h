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

#ifndef _EVAL_TEST_H
#define _EVAL_TEST_H

void machine_create_and_destroy();
void test_inc();
void test_mov_const_x();
void test_mov_const_mem();
void test_mov_const_s();
void test_add_c_c();
void test_eql_x_c();
void test_div_420_0_fail();
void test_mod_420_0_fail();
void test_joo();
void test_joz();
void test_soo();
void test_soz();
void test_jmp();
void test_jsr();
void test_nested_jsr();
void test_empty_jump_stack();

#endif
