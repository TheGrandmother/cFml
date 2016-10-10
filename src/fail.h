#include "e4c/e4c_lite.h"
#ifndef _FAIL_H
#define _FAIL_H

/** Crash the entire FML program.
 *
 * Used for erious errors when the entire program should die.
 * This should be depreciated eventually in favour of better
 * error handling.
 *
 * @param return_code The exit code.
 * @param message The message to be displayed.
 **/
void crash(int return_code, const char *message);

/** Crash the entire FML program and aborts.
 *
 * Used for erious errors when the entire program should die.
 *
 * This function calls abort instead of exit in order to preserve debug information.
 *
 * This should be depreciated eventually in favour of better
 * error handling.
 *
 * @param message The message to be displayed.
 **/
void crashAndBurn(const char *message);

#endif
