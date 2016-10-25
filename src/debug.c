#include "constants/reverse_lookup.h"
#include "constants/isa_constants.h"
#include "constants/operation_constants.h"
#include "components.h"
#include "types.h"
#include "exceptions.h"
#include "e4c/e4c_lite.h"

#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>

extern inline char *mne_lookup(fml_word op_index);
extern inline fml_addr arg_lookup(fml_word op_index);

#define BUF_SIZE 256

char *decode_arg(fml_ram *ram, fml_addr pc, fml_word a, fml_addr offs){
  char *pref = (char*)calloc(BUF_SIZE, sizeof(char));
  if((a & ADDRESS_MASK) != 0){
    strncpy(pref,"$",BUF_SIZE);
  }else if((a & SP_MASK ) != 0){
    strncpy(pref,"$$",BUF_SIZE);
  }

  char *val = (char*)calloc(BUF_SIZE, sizeof(char));
  switch((a & LOCATION_MASK)){
    case(ACC_STACK):
      strncpy(val,"s",BUF_SIZE);
      break;
    case(REG_X):
      strncpy(val,"x",BUF_SIZE);
      break;
    case(REG_Y):
      strncpy(val,"y",BUF_SIZE);
      break;
    case(CONSTANT):
      sprintf(val,"0x%lx",ram->prg_ram[pc+offs]);
      break;
    case(SP):
      sprintf(val,"sp");
      break;
    default:
      fprintf(stderr,"Tried to decode strange arguemnt.\n");
      E4C_THROW(FmlException,NULL);
  }

  char *ret = (char*)calloc(BUF_SIZE, sizeof(char));
  strncat(ret,pref,BUF_SIZE);
  strncat(ret,val,BUF_SIZE);
  free(pref);
  free(val);
  return ret;
}

char* decode_instruction(fml_ram *ram, fml_addr pc){

  fml_word instruction = ram->prg_ram[pc];
  char *ret = (char*)calloc(BUF_SIZE, sizeof(char));
  if(instruction == 0){
    strncpy(ret,"NOP",BUF_SIZE);
    return ret;
  }

  fml_word operation = instruction >> OPCODE_SHIFT;
  fml_word a0;
  char *a0_string;
  fml_word a1;
  char *a1_string;

  fml_addr args = arg_lookup(operation);
  strncat(ret,mne_lookup(operation),BUF_SIZE);

  if(args > 0){ 
    a0 = (instruction & A0_MASK) >> A0_SHIFT;
    a0_string = decode_arg(ram, pc, a0, 1);
    strncat(ret," ",BUF_SIZE);
    strncat(ret,a0_string,BUF_SIZE);
    free(a0_string);
  }
  if (args > 1){
      fml_addr a1_offs = A0_OFFS + 
        ((((instruction & A0_MASK) >> A0_SHIFT) & LOCATION_MASK) == CONSTANT  ?1 : 0);
    a1 = (instruction & A1_MASK) >> A1_SHIFT;
    a1_string = decode_arg(ram, pc, a1, a1_offs);
    strncat(ret," ",BUF_SIZE);
    strncat(ret,a1_string,BUF_SIZE);
    free(a1_string);
  }

  return ret;
}

void print_prog(fml_ram *ram, fml_addr pc){
  size_t big_buf = 2048;
  char *buf = calloc(big_buf,sizeof(char));
  char *head = calloc(BUF_SIZE,sizeof(char));

  fml_addr i  =  0;
  while(i < ram->prg_size){
    if(i == pc){
      sprintf(head,"-> 0x%lx: ",i);
    }else{
      sprintf(head,"   0x%lx: ",i);
    }
    char* instruction = decode_instruction(ram,i);
    strncat(buf,head,big_buf);
    strncat(buf,instruction,big_buf);
    strncat(buf,"\n",big_buf);
    free(instruction);
    //free(head);
    i += ((ram->prg_ram[i] & STEP_MASK) == 0) ? 1 : (ram->prg_ram[i] & STEP_MASK) >> STEP_SHIFT;
  }

  printf("%s",buf);
  free(buf);
}







