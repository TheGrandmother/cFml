#include "components.h"
#include "e4c/e4c_lite.h"

#ifndef _RAM_H
#define _RAM_H

E4C_DECLARE_EXCEPTION(RamException);
E4C_DECLARE_EXCEPTION(BadAddressException);
E4C_DECLARE_EXCEPTION(SpecialPageException);
E4C_DECLARE_EXCEPTION(VideoPageException);
E4C_DECLARE_EXCEPTION(HardwarePageException);
E4C_DECLARE_EXCEPTION(IrqPageException);

/** Creates and allocates space for a new ram object.
 * 
 * This function also creates new pages (not yet implemented)
 * @param size the size of the new ram..
 */
fml_ram *newRam(size_t size);

/** Destroys the ram object.
 *
 * @param ram The object to be destroyed.
 */
void destroyRam(fml_ram *ram);

/** Reads a word from the ram.
 * 
 * Calls the read functions of the special pages if needed
 * 
 * @param ram The ram object to be read from.
 * @param addr The address from which we are to read.
 * @return The word at the supplied address.
 */
fml_word read(fml_ram *ram, fml_addr addr);

/** Writes a word to the memory.
 * 
 * Calls the write functions of the special pages if needed
 * 
 * @param ram The ram object to be write to.
 * @param addr The address to which the data is to be written.
 * @param data The data to be written.
 */
void write(fml_ram *ram, fml_addr addr, fml_word data);
#endif
