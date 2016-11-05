#include "../types.h"

#ifndef _IO_PAGE_H
#define _IO_PAGE_H

void io_page_call(fml_machine *fml, fml_word key);
void io_page_write(fml_addr addr, fml_word data);
fml_word io_page_read(fml_addr addr);

#endif
