#include <stdio.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "stack_test.h"
#include "ram_test.h"
#include "eval_test.h"


#ifdef OLD_CUNIT
#define TEST_SUITE(NAME, SUITE_NAME) {NAME, init_suite, clean_suite, SUITE_NAME},
#else
#define TEST_SUITE(NAME, SUITE_NAME) {NAME, init_suite, clean_suite, set_up, tear_down, SUITE_NAME},
#endif

int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }
void set_up(void) {}
void tear_down(void) {}

int main(){

   /* initialize the CUnit test registry */
   if ( CUE_SUCCESS != CU_initialize_registry() )
      return CU_get_error();
    
   CU_TestInfo stack_tests[] = {
     {"Testing build and destroy.", stack_build_and_destroy},
     {"Standard stack test.", stack_standard_test},
     {"Test empty exception", test_empty_exception},
     {"Test Overflow exception", test_overflow_exception},
     {"Test single push and pop",test_single_push_n_pop},
     CU_TEST_INFO_NULL,
   };

   CU_TestInfo ram_tests[] = {
     {"Testing build and destroy.", create_and_destroy},
     {"Testing read and write.", write_and_read},
     {"Testing invalid address.", test_invalid_addr},
     {"Testing invalid page.", test_invalid_page},
     CU_TEST_INFO_NULL,
   };

   CU_TestInfo eval_tests[] = {
     {"Testing build and destroy.", machine_create_and_destroy},
     {"Testing INC x", test_inc},
     {"Testing MOV 420 x", test_mov_const_x},
     {"Testing MOV 420 $42", test_mov_const_mem},
     {"Testing MOV 420 s", test_mov_const_s},
     {"Testing ADD 20 400", test_add_c_c},
     {"Testing DIV 420 0", test_div_420_0_fail},
     {"Testing MOD 420 0", test_mod_420_0_fail},
     CU_TEST_INFO_NULL,
   };

   CU_SuiteInfo suites[] = {
     TEST_SUITE("Testing Stack", stack_tests)
     TEST_SUITE("Testing Ram"  , ram_tests)
     TEST_SUITE("Testing Eval" , eval_tests) 
     CU_SUITE_INFO_NULL,
   };

   /* add a suite to the registry */
   CU_register_suites(suites);

   // Run all tests using the basic interface
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   printf("\n");

   CU_basic_show_failures(CU_get_failure_list());
   CU_cleanup_registry();
   printf("\n");

   return CU_get_error();
}
