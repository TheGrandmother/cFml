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

#endif
