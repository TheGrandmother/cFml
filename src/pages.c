#include <stdlib.h>
#include <stdio.h>
#include "types.h"
#include "components.h"
#include "constants/pages/io_page_constants.h"
#include "constants/memmory_constants.h"
#include "pages/io_page.h"



void external_function_call(fml_machine *fml, fml_addr key){
  switch((key) >> PAGE_SHIFT){
    case(IO_PAGE_INDEX):
      io_page_call(fml,key & ~PAGE_MASK);
      break;
    default:
      fprintf(stderr,"Not a valid page :/\n");
      abort();
  }


}

