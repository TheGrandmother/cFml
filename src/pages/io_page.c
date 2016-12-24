#include <stdio.h>
#include "../types.h"
#include "../components.h"
#include "../stack.h"
#include "../constants/pages/io_page_constants.h"

void io_println(fml_machine *fml){
  fml_addr str_addr = pop(fml->s); 
  while(fml->ram->ram[str_addr] != 0){
    printf("%c",(int)(fml->ram->ram[str_addr]));
    str_addr++;
  }
  printf("\n");
}

void io_page_call(fml_machine *fml, fml_word key){

  switch(key){
    case(PRINTLN):
      io_println(fml);
      break;
    default:
      fprintf(stderr,"Not a valid IO function\n");
      abort();
      break;

  }
}


