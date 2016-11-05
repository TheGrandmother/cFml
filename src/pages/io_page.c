#include <stdio.h>
#include "../types.h"
#include "../components.h"
#include "../constants/pages/io_page_constants.h"

void io_page_call(fml_machine *fml, fml_word key){

  switch(key){
    case(PRINTLN):
      break;
    default:
      fprintf(stderr,"Not a valid function\n");
      abort();
      break;

  }
}

