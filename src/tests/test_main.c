#include <stdio.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "stack_test.h"
#include "ram_test.h"

int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }

int main(){
   CU_pSuite pSuite = NULL;

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
     {"Testing build and destroy.", createAndDestroy},
     {"Testing read and write.", writeAndRead},
     {"Testing invalid address.", testInvalidAddr},
     {"Testing invalid page.", testInvalidPage},
     CU_TEST_INFO_NULL,
   };

   CU_SuiteInfo suites[] = {
     { "Testing Stack", init_suite, clean_suite, stack_tests },
     { "Testing Ram", init_suite, clean_suite, ram_tests },
     CU_SUITE_INFO_NULL,
   };

   /* add a suite to the registry */
   CU_register_suites(suites);
   //if (CU_register_suites(suites)) {
   //   printf("DOING STUFF\n");
   //   CU_cleanup_registry();
   //   return CU_get_error();
   //}

   ///* add the tests to the suite */
   //if ( 
   //    (NULL == CU_add_test(pSuite, "Testing build and destroy.", stack_build_and_destroy)) ||
   //    (NULL == CU_add_test(pSuite, "Standard stack test.", stack_standard_test)) ||
   //    (NULL == CU_add_test(pSuite, "Test empty exception", testEmptyException)) ||
   //    (NULL == CU_add_test(pSuite, "Test Overflow exception", testOverflowException)) 
   //   )
   //{
   //   CU_cleanup_registry();
   //   return CU_get_error();
   //}

   // Run all tests using the basic interface
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   printf("\n");
   CU_basic_show_failures(CU_get_failure_list());
   //printf("\n\n");
/*
   // Run all tests using the automated interface
   CU_automated_run_tests();
   CU_list_tests_to_file();

   // Run all tests using the console interface
   CU_console_run_tests();
*/
   /* Clean up registry and return */
   CU_cleanup_registry();
   return CU_get_error();
}
