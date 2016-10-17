#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdbool.h>

#include "../e4c/e4c_lite.h"
#include "../ram.h"
#include "../types.h"
#include "../constants.h"
#include "../components.h"

void create_and_destroy(){
  fml_ram *ram = create_ram(10);
  destroy_ram(ram);
}

void write_and_read(){
  size_t size = 100;
  fml_ram *ram = create_ram(size);

  srand(time(NULL));
  for(int i = 0; i < 10000; i++){
    fml_addr addr = rand() % size;
    fml_word val = rand();
    E4C_TRY{
    write(ram, addr, val);
    if(read(ram,addr) != val){
      CU_FAIL("Did not read same value as written.");
    }
    }E4C_CATCH(RamException){
      CU_FAIL("Caught RamException");
    }
  }
  destroy_ram(ram);
}

void test_invalid_addr(){
  size_t size = 10;
  fml_ram *ram = create_ram(size);


  bool thrown = false;
  E4C_TRY{
    write(ram,(fml_addr)(size+1),1);
  }E4C_CATCH(BadAddressException){
    thrown = true; 
  }
  CU_ASSERT(thrown); //Check that correct exception was thrown

  thrown = false;
  E4C_TRY{
    read(ram,(fml_addr)(size+1));
  }E4C_CATCH(BadAddressException){
    thrown = true; 
  }
  CU_ASSERT(thrown); //Check that correct exception was thrown
}

void test_invalid_page(){
  size_t size = 10;
  fml_ram *ram = create_ram(size);

  bool thrown =false;
  E4C_TRY{
    write(ram,0x8000000000000000LL,8);
  }E4C_CATCH(SpecialPageException){
    thrown = true;
  }

  CU_ASSERT(thrown);
  
  thrown =false;
  E4C_TRY{
    read(ram,0x8000000000000000LL);
  }E4C_CATCH(SpecialPageException){
    thrown = true;
  }

  CU_ASSERT(thrown);
}



