#include <stdio.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "stack_test.h"

int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }

int main(){
   CU_pSuite pSuite = NULL;

   /* initialize the CUnit test registry */
   if ( CUE_SUCCESS != CU_initialize_registry() )
      return CU_get_error();

   /* add a suite to the registry */
   pSuite = CU_add_suite( "Stack tests", init_suite, clean_suite );
   if ( NULL == pSuite ) {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* add the tests to the suite */
   if ( 
       (NULL == CU_add_test(pSuite, "Testing build and destroy.", stack_build_and_destroy)) ||
       (NULL == CU_add_test(pSuite, "Standard stack test.", stack_standard_test)) ||
       (NULL == CU_add_test(pSuite, "Test empty exception", testEmptyException)) ||
       (NULL == CU_add_test(pSuite, "Test Overflow exception", testOverflowException)) 
      )
   {
      CU_cleanup_registry();
      return CU_get_error();
   }

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
