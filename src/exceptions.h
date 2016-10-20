#include "e4c/e4c_lite.h"

#ifndef _EXCEPTIONS_H
#define _EXCEPTIONS_H
E4C_DECLARE_EXCEPTION(FmlException);
E4C_DECLARE_EXCEPTION(NotImplementedException);
E4C_DECLARE_EXCEPTION(ArithmeticException);
E4C_DECLARE_EXCEPTION(InstructionException);
E4C_DECLARE_EXCEPTION(DivByZeroException);
E4C_DECLARE_EXCEPTION(ModByZeroException);
E4C_DECLARE_EXCEPTION(MoveToConstantException);
E4C_DECLARE_EXCEPTION(InvalidOperationException);

//E4C_DEFINE_EXCEPTION(FmlException, "General FML exception", RuntimeException);
//E4C_DEFINE_EXCEPTION(NotImplementedException, "Instruction Not Implemented", FmlException);
//E4C_DEFINE_EXCEPTION(ArithmeticException, "General Arithmetic exception", FmlException);
//E4C_DEFINE_EXCEPTION(DivByZeroException, "Div by 0 exception", ArithmeticException);
//E4C_DEFINE_EXCEPTION(ModByZeroException, "Mod by 0 exception", ArithmeticException);
//E4C_DEFINE_EXCEPTION(InstructionException, "General Instruction Exception", FmlException);
//E4C_DEFINE_EXCEPTION(MoveToConstantException, "Move To Constant Exception", InstructionException);
//E4C_DEFINE_EXCEPTION(InvalidOperationException, "Invalid Operation", InstructionException);
#endif
