/**
* This file contains the constats for all the instructions
* 
 * The file was auto generated. DO NOT EDIT!
**/

#ifndef _OPERATION_CONSTANTS_H
#define _OPERATION_CONSTANTS_H

#define NOP	0

//These are instruction hints
//Used to for performance
#define BINARY_START	0x4ULL
#define UNARY_START	0x11ULL
#define CONTROL_START	0x12ULL
#define OPERATIONS_END	0x1bULL

//These are the argument counts for the
//special and control operations
#define DEC_ARG_COUNT	0x1ULL
#define DIE_ARG_COUNT	0x0ULL
#define JOZ_ARG_COUNT	0x1ULL
#define HLT_ARG_COUNT	0x0ULL
#define MOV_ARG_COUNT	0x2ULL
#define RET_ARG_COUNT	0x0ULL
#define SOO_ARG_COUNT	0x1ULL
#define JSR_ARG_COUNT	0x1ULL
#define JOO_ARG_COUNT	0x1ULL
#define SOZ_ARG_COUNT	0x1ULL
#define JMP_ARG_COUNT	0x1ULL
#define INC_ARG_COUNT	0x1ULL

//These are the numeric values of the operations
#define GRT_VALUE	0x5ULL
#define XOR_VALUE	0x6ULL
#define SUB_VALUE	0x7ULL
#define DIE_VALUE	0x12ULL
#define RET_VALUE	0x16ULL
#define ADD_VALUE	0x8ULL
#define EQL_VALUE	0xaULL
#define MUL_VALUE	0xbULL
#define DIV_VALUE	0xcULL
#define DEC_VALUE	0x1ULL
#define NEQ_VALUE	0xdULL
#define AND_VALUE	0x4ULL
#define SOZ_VALUE	0x18ULL
#define JMP_VALUE	0x19ULL
#define JOZ_VALUE	0x13ULL
#define HLT_VALUE	0x14ULL
#define MOV_VALUE	0x2ULL
#define SOO_VALUE	0x15ULL
#define LES_VALUE	0x9ULL
#define JSR_VALUE	0x17ULL
#define NOT_VALUE	0x11ULL
#define JOO_VALUE	0x1aULL
#define INC_VALUE	0x3ULL
#define ROT_VALUE	0xeULL
#define OR_VALUE	0xfULL
#define MOD_VALUE	0x10ULL

#endif
