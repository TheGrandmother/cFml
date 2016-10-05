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

fml_ram *newRam(size_t size){
  fml_ram *ram = malloc(sizeof(fml_ram));
  ram->ram_size = size;
  ram->ram = calloc(size,sizeof(fml_word));
  //TODO: Implement pages
  ram->video_page = NULL;
  ram->irq_page = NULL;
  ram->hardware_page = NULL;
}

void destroyRam(fml_ram *ram){
  free(ram->ram);
  //TODO: Implement freeing of pages
  free(ram);
}


fml_word read(fml_ram *ram, fml_addr addr){
  if((addr & PAGE_MASK) == 0){
    return ram->ram[addr];
  }else{
    switch (addr & PAGE_MASK){
      case VIDEO_PAGE_VALUE:
        crashAndBurn("Video page not implemented for reading.");
        break;
      case HARDWARE_PAGE_VALUE:
        crashAndBurn("Hardware page not implemented for reading.");
        break;
      case IRQ_PAGE_VALUE:
        crashAndBurn("IRQ page not implemented for reading.");
        break;
      default:
        crashAndBurn("Invalid memory page for reading.");
        break;
    }
  }
}

void write(fml_ram *ram, fml_addr addr, fml_word data){
  if((addr & PAGE_MASK) == 0){
    ram->ram[addr] = data;
  }else{
    switch (addr & PAGE_MASK){
      case VIDEO_PAGE_VALUE:
        crashAndBurn("Video page not implemented for writing.");
        break;
      case HARDWARE_PAGE_VALUE:
        crashAndBurn("Hardware page not implemented for writing.");
        break;
      case IRQ_PAGE_VALUE:
        crashAndBurn("IRQ page not implemented for writing.");
        break;
      default:
        crashAndBurn("Invalid memory page for writing.");
        break;
    }
  }
}
