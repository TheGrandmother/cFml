/**
 * This file contains the implementation of the FMLs memmory.
 *
 * TODO: Do some sane way of handling the DMA pages.
 **/
#include "components.h"
#include "constants.h"
#include "types.h"
#include "fail.h"
#include "video_page.h"
#include "hardware_page.h"
#include "video_page.h"
#include "irq_page.h"
#include "e4c/e4c_lite.h"

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
      case VIDEO_PAGE_VALUE:
        E4C_THROW(VideoPageException, "Video page not implemented");
        break;
      case HARDWARE_PAGE_VALUE:
        E4C_THROW(HardwarePageException, "Hardware page not implemented");
        break;
      case IRQ_PAGE_VALUE:
        E4C_THROW(IrqPageException, "IRQ page not implemented");
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
      case VIDEO_PAGE_VALUE:
        E4C_THROW(VideoPageException, "Video page not implemented");
        break;
      case HARDWARE_PAGE_VALUE:
        E4C_THROW(HardwarePageException, "Hardware page not implemented");
        break;
      case IRQ_PAGE_VALUE:
        E4C_THROW(IrqPageException, "IRQ page not implemented");
        break;
      default:
        E4C_THROW(SpecialPageException,"Invalid Special Page");
        break;
    }
  }
}
