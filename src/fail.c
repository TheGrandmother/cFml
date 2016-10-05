/**
 * This file contains the error handling of for the VM.
 *
 * TODO: Fix a better and more general failing for the VM.
 *       Preferably in a way such that one can get additional 
 *       information.
 *
 * TODO: Document all returncodes.
 *
 **/
#include <stdio.h>
#include <stdlib.h>


void crash(int return_code, const char *message){
  fprintf(stderr,"CRASH!\n%s\nProgram terminated.",message);
  exit(return_code);
}

void crashAndBurn(const char *message){
  fprintf(stderr,"CRASH!\n%s\nProgram terminated.",message);
  abort();
}
