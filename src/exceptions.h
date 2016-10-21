#include "e4c/e4c_lite.h"

#ifndef _EXCEPTIONS_H
#define _EXCEPTIONS_H
E4C_DECLARE_EXCEPTION(FmlException);
E4C_DECLARE_EXCEPTION(NotImplementedException);
E4C_DECLARE_EXCEPTION(EmptyJumpStackException);
E4C_DECLARE_EXCEPTION(SuicideException); //Awesome band name

E4C_DECLARE_EXCEPTION(ArithmeticException);
E4C_DECLARE_EXCEPTION(DivByZeroException);
E4C_DECLARE_EXCEPTION(ModByZeroException);

E4C_DECLARE_EXCEPTION(InstructionException);
E4C_DECLARE_EXCEPTION(MoveToConstantException);
E4C_DECLARE_EXCEPTION(InvalidOperationException);

#endif
