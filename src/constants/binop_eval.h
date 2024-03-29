/**
* Thise file contains auto generated code for the
* eval function.
* This file is generated from instructions.json.
*
* DO NOT EDIT THIS FILE
**/

#include "../types.h"
#include "../exceptions.h"
#include "../e4c/e4c_lite.h"
#include "../constants/isa_constants.h"
#include "../constants/operation_constants.h"

#include <stdio.h>

#ifndef _BINOP_EVAL_H
#define _BINOP_EVAL_H

inline fml_word eval_binop(fml_word op_index,fml_word a0, fml_word a1){
	fml_word ret = 0;

	switch(op_index){
	case(AND_VALUE):
		ret = a0 & a1;
		break;

	case(GRT_VALUE):
		ret = a0 > a1;
		break;

	case(XOR_VALUE):
		ret = a0 ^ a1;
		break;

	case(SUB_VALUE):
		ret = a0 - a1;
		break;

	case(ADD_VALUE):
		ret = a0 + a1;
		break;

	case(LES_VALUE):
		ret = a0 < a1;
		break;

	case(EQL_VALUE):
		ret = a0 == a1;
		break;

	case(MUL_VALUE):
		ret = a0 * a1;
		break;

	case(DIV_VALUE):
		if(a1 != 0){
			ret = a0 / a1;
		}else{
			fprintf(stderr,"Division by zero\n"); E4C_THROW(DivByZeroException, NULL);
		}
		break;

	case(NEQ_VALUE):
		ret = a0 != a1;
		break;

	case(ROT_VALUE):
		ret = (a1 > 0) ? a0 << a1 : a0 >> a1;
		break;

	case(OR_VALUE):
		ret = a0 | a1;
		break;

	case(MOD_VALUE):
		if(a1 != 0){
			ret = a0 % a1;
		}else{
			fprintf(stderr,"Mod by zero\n"); E4C_THROW(ModByZeroException, NULL);
		}
		break;

	}

	return ret;
}

#endif
