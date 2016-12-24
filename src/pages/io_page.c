#include <stdio.h>
#include <stdbool.h>
#include "../types.h"
#include "../components.h"
#include "../stack.h"
#include "../constants/pages/io_page_constants.h"

void io_println(fml_machine *fml,bool new_line){
  fml_addr str_addr = pop(fml->s); 
  while(fml->ram->ram[str_addr] != 0){
    printf("%c",(int)(fml->ram->ram[str_addr]));
    str_addr++;
  }
  if(new_line){
    printf("\n");
  }
}

void io_flush_in(){
  char c;
  while ((c = getchar()) != '\n' && c != EOF) { }
}

void io_readline(fml_machine *fml){
  fml_int size = pop(fml->s);
  fml_addr buff_start = pop(fml->s);
  fml_int index = 0;

  char c;
  do{
     c = getchar();
     fml->ram->ram[buff_start+index] = c;
     index++;
  }while((index < size-1 && (c != '\n' && c != EOF)));

  fml->ram->ram[buff_start+index+1] = '\0';
  if((c != '\n' && c != EOF)){
    io_flush_in();
  }


}



void io_get_char(fml_machine *fml){
  push(fml->s, getchar());
}

void io_put_char(fml_machine *fml){
  putchar((char)pop(fml->s));
}

void io_page_call(fml_machine *fml, fml_word key){

  switch(key){
    case(PRINTLN):
      io_println(fml, true);
      break;
    case(PRINT):
      io_println(fml, false);
      break;
    case(GET_CHAR):
      io_get_char(fml);
      break;
    case(PUT_CHAR):
      io_put_char(fml);
      break;
    case(FLUSH_IN):
      io_flush_in();
      break;
    case(READ_LINE):
      io_readline(fml);
      break;
    default:
      fprintf(stderr,"Not a valid IO function\n");
      abort();
      break;

  }
}


