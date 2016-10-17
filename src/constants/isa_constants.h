/**
* This file contains the numerical constants for the
* instructions set architecture
* 
* This file was automatically generated.
* DO NOT EDIT THIS FILE DIRECTLY!
**/

#ifndef _ISA_CONSTANTS_H
#define _ISA_CONSTANTS_H

//The following constnts concern the instruction layout
#define STEP_MASK 	0x0000000000000700ULL
#define A0_MASK 	0x000000000000000fULL
#define A1_MASK 	0x00000000000000f0ULL
#define OPCODE_MASK 	0xfffffffffffff800ULL
#define A1_SHIFT 	0x4U
#define STEP_SHIFT 	0x8U
#define A0_SHIFT 	0x0U
#define OPCODE_SHIFT 	0xbU
#define A0_OFFS	0x1U
#define A1_OFFS	0x2U


//These constants define the strucuture of the argument
#define SP_MASK 	0x0000000000000008ULL
#define LOCATION_MASK 	0x0000000000000003ULL
#define ADDRESS_MASK 	0x0000000000000004ULL
#define REG_X 	0x0U
#define REG_Y 	0x1U
#define CONSTANT 	0x3U
#define ACC_STACK 	0x2U


#endif
