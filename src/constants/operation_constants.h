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
#define BINARY_START	0x5U
#define UNARY_START	0x11U
#define CONTROL_START	0x12U
#define OPERATIONS_END	0x19U

//These are the argument counts for the
//special and control operations
#define DEC_ARG_COUNT	0x1U
#define JMP_ARG_COUNT	0x1U
#define HLT_ARG_COUNT	0x0U
#define MOV_ARG_COUNT	0x2U
#define RET_ARG_COUNT	0x0U
#define SOO_ARG_COUNT	0x1U
#define JSR_ARG_COUNT	0x1U
#define JOO_ARG_COUNT	0x1U
#define SOZ_ARG_COUNT	0x1U
#define JOZ_ARG_COUNT	0x1U
#define INC_ARG_COUNT	0x1U

//These are the numeric values of the operations
#define GRT_VALUE	0x5U
#define XOR_VALUE	0x6U
#define SUB_VALUE	0x7U
#define RET_VALUE	0x14U
#define ADD_VALUE	0xfU
#define EQL_VALUE	0xaU
#define MUL_VALUE	0xbU
#define DIV_VALUE	0xcU
#define DEC_VALUE	0x1U
#define AND_VALUE	0x4U
#define JMP_VALUE	0x17U
#define JOZ_VALUE	0x11U
#define HLT_VALUE	0x12U
#define MOV_VALUE	0x2U
#define SOO_VALUE	0x13U
#define LES_VALUE	0x9U
#define JSR_VALUE	0x15U
#define NOT_VALUE	0x10U
#define JOO_VALUE	0x18U
#define SOZ_VALUE	0x16U
#define ROT_VALUE	0xdU
#define OR_VALUE	0xeU
#define INC_VALUE	0x3U

#endif
