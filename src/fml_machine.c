#include "constants/isa_constants.h"
#include "constants/operation_constants.h"
#include "components.h"
#include "stack.h"
#include "ram.h"

fml_machine *create_machine(size_t ram_size, size_t prg_size, size_t acc_size, size_t js_size){
  fml_machine *fml = (fml_machine*)calloc(1,sizeof(fml_machine));
  fml->s = new_stack(acc_size);
  fml->js = new_stack(js_size);
  fml->ram = create_ram(ram_size, prg_size);
  return fml;
}

void soft_reset(fml_machine *fml){
  fml->pc = 0;
  fml->halt = 0;
  fml->sp = 0;
  fml->irq_interupt = 0;
}

void hard_reset(fml_machine *fml){
  fml->pc = 0;
  fml->halt = 0;
  fml->sp = 0;
  fml->irq_interupt = 0;
  fml_addr stack_size = fml->s->size;
  fml_addr js_size = fml->js->size;
  fml_addr ram_size = fml->ram->ram_size;
  fml_addr prg_size = fml->ram->prg_size;
  destroy_ram(fml->ram);
  destroy_stack(fml->js);
  destroy_stack(fml->s);
  fml->s = new_stack(stack_size);
  fml->js = new_stack(js_size);
  fml->ram = create_ram(ram_size, prg_size);
}

void destroy_machine(fml_machine *fml){
  destroy_ram(fml->ram);
  destroy_stack(fml->js);
  destroy_stack(fml->s);
  free(fml);
}
