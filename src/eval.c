#include "e4c/e4c_lite.h"
#include "constants/isa_constants.h"
#include "constants/operation_constants.h"
#include "constants/binop_eval.h"
#include "exceptions.h"
#include "components.h"
#include "stack.h"
#include "ram.h"
#include "debug.h"


#include <stdio.h>


E4C_DEFINE_EXCEPTION(FmlException, "General FML exception", RuntimeException);
E4C_DEFINE_EXCEPTION(NotImplementedException, "Instruction Not Implemented", FmlException);
E4C_DEFINE_EXCEPTION(EmptyJumpStackException, "The jump stack is empty", FmlException);
E4C_DEFINE_EXCEPTION(SuicideException, "The VM is no more", FmlException);

E4C_DEFINE_EXCEPTION(ArithmeticException, "General Arithmetic exception", FmlException);
E4C_DEFINE_EXCEPTION(DivByZeroException, "Div by 0 exception", ArithmeticException);
E4C_DEFINE_EXCEPTION(ModByZeroException, "Mod by 0 exception", ArithmeticException);

E4C_DEFINE_EXCEPTION(InstructionException, "General Instruction Exception", FmlException);
E4C_DEFINE_EXCEPTION(MoveToConstantException, "Move To Constant Exception", InstructionException);
E4C_DEFINE_EXCEPTION(InvalidOperationException, "Invalid Operation", InstructionException);

extern inline fml_word eval_binop(fml_word, fml_word, fml_word);

fml_word read_argument(fml_word arg, fml_machine *self, fml_addr offs){
  fml_word val;
  switch(arg & LOCATION_MASK){
    case REG_X:
      val = self->x;
      break;
    case REG_Y:
      val = self->y;
      break;
    case ACC_STACK:
      val = pop(self->s);
      break;
    case CONSTANT:
      val = self->ram->prg_ram[self->pc + offs];
      break;
    case SP:
      val = self->sp;
      break;
    default:
      fprintf(stderr,"Undefined argument location !?");
      E4C_THROW(FmlException,NULL);
      break;
  }

  if((arg & ADDRESS_MASK) != 0){
    val += (arg & SP_MASK) != 0 ? self->sp : 0;  
    return read(self->ram,val);
  }
  return val;
  
}

void write_argument(fml_word arg, fml_machine *self, fml_addr offs, fml_word val){
  if((arg & ADDRESS_MASK) != 0){
    fml_addr addr = read_argument(arg & LOCATION_MASK, self, offs);
    addr += (arg & SP_MASK) != 0 ? self->sp : 0;  
    write(self->ram,addr,val);
  }else{
    switch(arg & LOCATION_MASK){
      case REG_X:
        self->x = val;
        break;
      case REG_Y:
        self->y = val;
        break;
      case ACC_STACK:
        push(self->s,val);
        break;
      case CONSTANT:
        fprintf(stderr,"Attempting to write to constant.\n");
        E4C_THROW(MoveToConstantException, NULL);
        break;
      case SP:
        self->sp = val;
        break;
      default:
        fprintf(stderr,"Undefined argument location !?\n");
        E4C_THROW(FmlException,NULL);
        break;
    }
  }
}

extern inline void eval_control(fml_machine *self, fml_word op_index, fml_word instruction, fml_addr step_length, fml_addr a1_offs, uint64_t steps){
  switch(op_index){
    fml_word a0;
    case(JMP_VALUE):
      a0 = read_argument((instruction & A0_MASK) >> A0_SHIFT, self, A0_OFFS);
      self->pc = a0;
      break;

    case(JOO_VALUE):
      a0 = read_argument((instruction & A0_MASK) >> A0_SHIFT, self, A0_OFFS);
      self->pc = (pop(self->s) == 1) ? a0 : self->pc + step_length;
      break;

    case(JOZ_VALUE):
      a0 = read_argument((instruction & A0_MASK) >> A0_SHIFT, self, A0_OFFS);
      self->pc = (pop(self->s) == 0) ? a0 : self->pc + step_length;
      break;

    case(SOO_VALUE):
      if(pop(self->s) == 1){
        goto exec_jsr;
      }else{
        self->pc += step_length;
      }
      break;

    case(SOZ_VALUE):
      if(pop(self->s) == 0){
        goto exec_jsr;
      }else{
        self->pc += step_length;
      }
      break;

    case(JSR_VALUE):
exec_jsr:
      a0 = read_argument((instruction & A0_MASK) >> A0_SHIFT, self, A0_OFFS);
      push(self->js,self->pc+step_length);
      self->pc = a0;
      break;

    case(RET_VALUE):
      E4C_TRY{
        self->pc = pop(self->js);
      }E4C_CATCH(StackEmptyException){
        fprintf(stderr,"Jump stack is empty.\n");
        E4C_THROW(EmptyJumpStackException, NULL);

      }
      break;

    case(HLT_VALUE):
      self->halt = 1;
      printf("Halting VM after %lu steps\n",steps);
      break;

    case(DIE_VALUE):
      E4C_THROW(SuicideException,NULL);
      self->halt = 1;
      break;

    case(EFC_VALUE):
      E4C_THROW(SuicideException,NULL);
      self->halt = 1;
      break;

    default:
      fprintf(stderr,"Not implemented");
      E4C_THROW(NotImplementedException,NULL);
      break;
  }

}

extern inline void eval_special(fml_machine *self, fml_word op_index, fml_word instruction, fml_addr step_length, fml_addr a1_offs){
  fml_word val = read_argument((instruction & A0_MASK) >> A0_SHIFT, self, A0_OFFS);

  switch(op_index){
    case(INC_VALUE):
      write_argument((instruction & A0_MASK) >> A0_SHIFT, self, A0_OFFS, val + 1);
      break;
    case(DEC_VALUE):
      write_argument((instruction & A0_MASK) >> A0_SHIFT, self, A0_OFFS, val - 1);
      break;
    case(MOV_VALUE):
      write_argument((instruction & A1_MASK) >> A1_SHIFT, self, a1_offs, val);
      break;
    default:
      fprintf(stderr,"No such special Operation.");
      E4C_THROW(InstructionException,NULL);
      break;
  }
}

int eval(fml_machine *self, uint64_t max_steps){
  puts("Entered eval function");
  uint64_t steps = 0;
  while(!self->halt){

    fml_word instruction = self->ram->prg_ram[self->pc];

//#define DEBUG_FULL
#ifdef DEBUG_FULL
    puts("");
    printf("Evaluating at 0x%lx\n",self->pc);
    print_prog(self->ram, self->pc);
    print_ram(self->ram, 0, 10);
    print_stack(self->s);
    print_stack(self->js);
#endif

    fml_addr step_length = (instruction & STEP_MASK) >> STEP_SHIFT;

    if(instruction != NOP){
      fml_word op_index = (instruction & OPCODE_MASK) >> OPCODE_SHIFT;

      //Compute offs
      fml_addr a1_offs = A0_OFFS + 
        ((((instruction & A0_MASK) >> A0_SHIFT) & LOCATION_MASK) == CONSTANT  ? 1 : 0);

      if(op_index < CONTROL_START){
        if(op_index < BINARY_START){
          //Special Operations
          eval_special(self, op_index, instruction, step_length, a1_offs);
        
        }else if(op_index < UNARY_START){
          fml_word a0 = read_argument((instruction & A0_MASK) >> A0_SHIFT, self, A0_OFFS);
          fml_word a1 = read_argument((instruction & A1_MASK) >> A1_SHIFT, self, a1_offs);
          push(self->s,eval_binop(op_index, a0, a1));

        }else{
          fprintf(stderr,"Unary operations not implemented");
          E4C_THROW(NotImplementedException,NULL);
        }

        self->pc += step_length;

      }else if(op_index < OPERATIONS_END){
        //Control operations. 
        eval_control(self, op_index, instruction, step_length, a1_offs, steps);
      }else{
          fprintf(stderr,"Operation out of bounds: %lu(%lX)\n",op_index,op_index);
          E4C_THROW(InvalidOperationException,NULL);
          return 1;
      }
    }else{
      self->pc++;
    }

    if(steps != max_steps){
      steps++;
    }else{
      fprintf(stderr, "Exceeding maximum steps.\n");
      return 2;
    }
  }
  return 0;
}



