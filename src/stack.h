#include "components.h"
#include "fail.h"

#ifndef _STACK_H
#define _STACK_H

/** Allocates and intializes a new stack.
 *
 * @param size The size of the stack in fml_words
 * @return The new stack.
 */
fml_stack *new_stack(size_t size);

/** Pushes data onto the stack.
 *
 * Crashes the program on stack overflow.
 * TODO: Implement better error handling.
 *
 * @param stack The stack to be pused to.
 * @param data The data to be pushed.
 */
void push(fml_stack *stack, fml_word data);

/** Pops an element of the stack.
 *
 * Crashes the entire VM if stack is empty.
 * TODO: Implement better error handling.
 *
 * @param The stack to be popped.
 * @return The data on the head of the stack.
 */
fml_word pop(fml_stack *stack);

/** Returns the head of the stack without removing it.
 *
 * @param stack the stack to be peeped.
 * @return The head of the stack.
 */
fml_word peep(fml_stack *stack);

/** Frees the stack.
 * @param the stack to be freed.
 */
void destroy_stack(fml_stack *stack);
#endif
