#include "components.h"
#include "stack.h"
#include "ram.h"

#ifndef _FML_MACHINE_H
#define _FML_MACHINE_H

fml_machine *create_machine(size_t ram_size, size_t prg_size, size_t acc_size, size_t js_size);
void destroy_machine(fml_machine *fml);

#endif

