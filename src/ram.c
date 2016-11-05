/**
 * This file contains the implementation of the FMLs memmory.
 *
 * TODO: Do some sane way of handling the DMA pages.
 **/
#include "components.h"
#include "constants/memmory_constants.h"
#include "constants/pages/io_page_constants.h"
#include "types.h"
#include "fail.h"
#include "pages/io_page.h"
#include "e4c/e4c_lite.h"

#include <stdio.h>
#include <string.h>

E4C_DEFINE_EXCEPTION(RamException, "Stack Exception.", RuntimeException);
E4C_DEFINE_EXCEPTION(BadAddressException, "Stack empty.", RamException);
E4C_DEFINE_EXCEPTION(SpecialPageException, "Exception in special page",RamException);
E4C_DEFINE_EXCEPTION(VideoPageException, "Exception in videopage", SpecialPageException);
E4C_DEFINE_EXCEPTION(HardwarePageException, "Exception in hardware page.", SpecialPageException);
E4C_DEFINE_EXCEPTION(IrqPageException, "Exception in IRQ page.", SpecialPageException);

fml_ram *create_ram(size_t size, size_t prg_size){
  fml_ram *ram = malloc(sizeof(fml_ram));
  ram->ram_size = (fml_addr)size;
  ram->prg_size = (fml_addr)prg_size;
  ram->ram = calloc(size,sizeof(fml_word));
  ram->prg_ram = calloc(prg_size,sizeof(fml_word));
  //TODO: Implement pages
  ram->video_page = NULL;
  ram->irq_page = NULL;
  ram->hardware_page = NULL;
  return ram;
}

void destroy_ram(fml_ram *ram){
  free(ram->ram);
  free(ram->prg_ram);
  //TODO: Implement freeing of pages
  free(ram);
}


fml_word read(fml_ram *ram, fml_addr addr){
  if((addr & PAGE_MASK) == 0){
    if(addr >= ram->ram_size){
      E4C_THROW(BadAddressException,"Address beyond memmory bounds.");
    }
    return ram->ram[addr];
  }else{
    switch (addr & PAGE_MASK){
      case IO_PAGE_INDEX:
        return io_page_read(addr);
        E4C_THROW(VideoPageException, "Video page not implemented");
        break;
      default:
        E4C_THROW(SpecialPageException,"Invalid Special Page");
        break;
    }
    return 0; //Silence warnings
  }
}

void write(fml_ram *ram, fml_addr addr, fml_word data){
  if((addr & PAGE_MASK) == 0){
    if(addr >= ram->ram_size){
      E4C_THROW(BadAddressException,"Address beyond memmory bounds.");
    }
    ram->ram[addr] = data;
  }else{
    switch (addr & PAGE_MASK){
      case IO_PAGE_INDEX:
        io_page_write(addr, data);
        break;
      default:
        E4C_THROW(SpecialPageException,"Invalid Special Page");
        break;
    }
  }
}


void print_ram(fml_ram *ram, fml_addr start, fml_addr stop){
  stop = (stop >= ram->ram_size) ? ram->ram_size - 1 : stop;

  char *string = calloc(1024, sizeof(char));
  for(fml_addr i = start;  i < stop; i++){
    char *tmp = calloc(256, sizeof(char));
    sprintf(tmp, "%lx : %lx(%lu)\n", i, ram->ram[i], ram->ram[i]);
    strcat(string,tmp);
  }
  printf("%s",string);

}
