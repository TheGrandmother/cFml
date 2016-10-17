#include <stdio.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "stack_test.h"
#include "ram_test.h"
#include "eval_test.h"

int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }

int main(){

   /* initialize the CUnit test registry */
   if ( CUE_SUCCESS != CU_initialize_registry() )
      return CU_get_error();
    
   CU_TestInfo stack_tests[] = {
     {"Testing build and destroy.", stack_build_and_destroy},
     {"Standard stack test.", stack_standard_test},
     {"Test empty exception", testEmptyException},
     {"Test Overflow exception", testOverflowException},
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
     {"Testing Inc", test_inc},
     CU_TEST_INFO_NULL,
   };

   CU_SuiteInfo suites[] = {
     { "Testing Stack", init_suite, clean_suite, stack_tests},
     { "Testing Ram", init_suite, clean_suite, ram_tests},
     { "Testing Eval", init_suite, clean_suite, eval_tests},
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
