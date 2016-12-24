#include <stdio.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "stack_test.h"
#include "ram_test.h"
#include "eval_test.h"
#include "io_tests.h"


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
    
   CU_TestInfo output_tests[] = {
     {"Testing print line.", test_println},
     CU_TEST_INFO_NULL,
   };

   CU_SuiteInfo suites[] = {
     TEST_SUITE("Testing output", output_tests)
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
