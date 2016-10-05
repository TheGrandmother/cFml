/**
 * This is the structs representing the different components of the FML vm.
 *
 * The structs are exposed to avoid having to use getters and setters
 *
 **/

#include "types.h"
#include "video_page.h"
#include "hardware_page.h"
#include "video_page.h"
#include "irq_page.h"

#ifndef _COMPONENTS_H
#define _CONPONENTS_H

typedef struct{
  fml_word* array;
  size_t index;
  size_t size;
}fml_stack;

typedef struct{
  fml_word* ram;
  size_t ram_size;
  fml_video_page* video_page;
  fml_irq_page* irq_page;
  fml_hardware_page* hardware_page;
}fml_ram;


typedef struct{
  fml_word x_reg;
  fml_word y_reg;

  fml_addr frame_pointer;
  fml_addr program_counter;

  fml_stack acumulator_stack;
  fml_stack jump_stack;

  fml_flag halt; 
  fml_flag irq_interupt;

}fml_machine;

#endif