/**
 * These are the ISA constants.
 *
 * Unfortunatley they have to be macros since they must be unsigned.
 **/
#ifndef _MASKS_H
#define _MASKS_H
//INSTRUCTION STRUCTURE CONSTANTS
#define   ARG0_MASK         0xFu
#define   ARG1_MASK         0xF0u
#define   OPERATION_MASK    0xF00u
#define   ACTION_MASK       0xF000u
#define   SIZE_MASK         0x30000u
#define   ARG0_SHIFT        0u
#define   ARG1_SHIFT        4u
#define   OPERATION_SHIFT   8u
#define   ACTION_SHIFT      12u
#define   SIZE_SHIFT        16u

//ARGUMENT CONSTANTS
#define   LOCATION_MASK     0x3
#define   IS_ADRESS         0x4
#define   IS_RELATIVE       0x8
#define   X_REG_VAL         0x0
#define   Y_RRG_VAL         0x1
#define   STACK_VAL         0x2
#define   CONST_VAL         0x3
#define   IS_ADDRESS_VAL    0x4
#define   IS_RELATIVE_VAL   0x8

//INSTRUCTION CONSTANTS

//OPERATIONS
#define NOP_VALUE 0   
#define INC_VALUE 1   
#define DEC_VALUE 2   
#define ADD_VALUE 3   
#define SUB_VALUE 4   
#define MUL_VALUE 5   
#define DIV_VALUE 6   
#define MOD_VALUE 7   
#define EQL_VALUE 8   
#define GRT_VALUE 9   
#define LES_VALUE 10  
#define AND_VALUE 11  
#define OOR_VALUE 12  
#define XOR_VALUE 13  
#define NOT_VALUE 14  
#define SFT_VALUE 15  
	
#define NOP_ARG_COUNT 0   
#define INC_ARG_COUNT 1   
#define DEC_ARG_COUNT 1   
#define ADD_ARG_COUNT 2   
#define SUB_ARG_COUNT 2   
#define MUL_ARG_COUNT 2   
#define DIV_ARG_COUNT 2   
#define MOD_ARG_COUNT 2   
#define EQL_ARG_COUNT 2   
#define GRT_ARG_COUNT 2   
#define LES_ARG_COUNT 2  
#define AND_ARG_COUNT 2  
#define OOR_ARG_COUNT 2  
#define XOR_ARG_COUNT 2  
#define NOT_ARG_COUNT 1  
#define SFT_ARG_COUNT 2  

//ACTIONS
#define NOP_VALUE 0   
#define JMP_VALUE 1   
#define JSR_VALUE 2   
#define RET_VALUE 3   
#define SEQ_VALUE 4   
#define SGR_VALUE 5   
#define SLE_VALUE 6   
#define JOO_VALUE 7   
#define JOZ_VALUE 8   
#define SOO_VALUE 9   
#define SOZ_VALUE 10  
#define HLT_VALUE 11  
#define MOV_VALUE 12  
#define SNE_VALUE 13  

#define NOP_ARG_COUNT 0   
#define JMP_ARG_COUNT 1   
#define JSR_ARG_COUNT 1   
#define RET_ARG_COUNT 0   
#define SEQ_ARG_COUNT 2   
#define SGR_ARG_COUNT 2   
#define SLE_ARG_COUNT 2   
#define JOO_ARG_COUNT 0   
#define JOZ_ARG_COUNT 0   
#define SOO_ARG_COUNT 0   
#define SOZ_ARG_COUNT 0  
#define HLT_ARG_COUNT 0  
#define MOV_ARG_COUNT 2 //WHAT ???? 
#define SNE_ARG_COUNT 2  

//RAM MASKS
#define PAGE_MASK 0xf000000000000000LL
#define VIDEO_PAGE_VALUE 0x1000000000000000LL
#define HARDWARE_PAGE_VALUE 0x2000000000000000LL
#define IRQ_PAGE_VALUE 0x3000000000000000LL

#endif
