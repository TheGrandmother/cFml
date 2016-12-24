/**
* Thise file contains auto generated code for 
* reverse mnemonic lookup.
* This file is generated from instructions.json.
*
* DO NOT EDIT THIS FILE
**/

#include "../types.h"
#include "../constants/isa_constants.h"
#include "../constants/operation_constants.h"
#ifndef _REVERSE_LOOKUP_H
#define _REVERSE_LOOKUP_H

inline char *mne_lookup(fml_word op_index){
	switch(op_index){
	case(DEC_VALUE):
		 return "DEC";

	case(MOV_VALUE):
		 return "MOV";

	case(INC_VALUE):
		 return "INC";

	case(AND_VALUE):
		 return "AND";

	case(GRT_VALUE):
		 return "GRT";

	case(XOR_VALUE):
		 return "XOR";

	case(SUB_VALUE):
		 return "SUB";

	case(ADD_VALUE):
		 return "ADD";

	case(LES_VALUE):
		 return "LES";

	case(EQL_VALUE):
		 return "EQL";

	case(MUL_VALUE):
		 return "MUL";

	case(DIV_VALUE):
		 return "DIV";

	case(NEQ_VALUE):
		 return "NEQ";

	case(ROT_VALUE):
		 return "ROT";

	case(OR_VALUE):
		 return "OR";

	case(MOD_VALUE):
		 return "MOD";

	case(NOT_VALUE):
		 return "NOT";

	case(EFC_VALUE):
		 return "EFC";

	case(DIE_VALUE):
		 return "DIE";

	case(JOZ_VALUE):
		 return "JOZ";

	case(HLT_VALUE):
		 return "HLT";

	case(SOO_VALUE):
		 return "SOO";

	case(RET_VALUE):
		 return "RET";

	case(JSR_VALUE):
		 return "JSR";

	case(SOZ_VALUE):
		 return "SOZ";

	case(JMP_VALUE):
		 return "JMP";

	case(JOO_VALUE):
		 return "JOO";

	}
  return "???";
}
inline fml_addr arg_lookup(fml_word op_index){
	switch(op_index){
	case(AND_VALUE):
		return 2;
		break;

	case(GRT_VALUE):
		return 2;
		break;

	case(XOR_VALUE):
		return 2;
		break;

	case(SUB_VALUE):
		return 2;
		break;

	case(ADD_VALUE):
		return 2;
		break;

	case(LES_VALUE):
		return 2;
		break;

	case(EQL_VALUE):
		return 2;
		break;

	case(MUL_VALUE):
		return 2;
		break;

	case(DIV_VALUE):
		return 2;
		break;

	case(NEQ_VALUE):
		return 2;
		break;

	case(ROT_VALUE):
		return 2;
		break;

	case(OR_VALUE):
		return 2;
		break;

	case(MOD_VALUE):
		return 2;
		break;

	case(NOT_VALUE):
		return 1;
		break;

	case(EFC_VALUE):
		return 1;
		break;

	case(DIE_VALUE):
		return 0;
		break;

	case(JOZ_VALUE):
		return 1;
		break;

	case(HLT_VALUE):
		return 0;
		break;

	case(SOO_VALUE):
		return 1;
		break;

	case(RET_VALUE):
		return 0;
		break;

	case(JSR_VALUE):
		return 1;
		break;

	case(SOZ_VALUE):
		return 1;
		break;

	case(JMP_VALUE):
		return 1;
		break;

	case(JOO_VALUE):
		return 1;
		break;

	case(DEC_VALUE):
		return 1;
		break;

	case(MOV_VALUE):
		return 2;
		break;

	case(INC_VALUE):
		return 1;
		break;

	}

return 0;
}

#endif
