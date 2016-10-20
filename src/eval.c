#include "e4c/e4c_lite.h"
#include "constants/isa_constants.h"
#include "constants/operation_constants.h"
#include "constants/binop_eval.h"
#include "exceptions.h"
#include "components.h"
#include "stack.h"
#include "ram.h"
#include <stdio.h>
E4C_DEFINE_EXCEPTION(FmlException, "General FML exception", RuntimeException);
E4C_DEFINE_EXCEPTION(NotImplementedException, "Instruction Not Implemented", FmlException);
E4C_DEFINE_EXCEPTION(ArithmeticException, "General Arithmetic exception", FmlException);
E4C_DEFINE_EXCEPTION(DivByZeroException, "Div by 0 exception", ArithmeticException);
E4C_DEFINE_EXCEPTION(ModByZeroException, "Mod by 0 exception", ArithmeticException);
E4C_DEFINE_EXCEPTION(InstructionException, "General Instruction Exception", FmlException);
E4C_DEFINE_EXCEPTION(MoveToConstantException, "Move To Constant Exception", InstructionException);
E4C_DEFINE_EXCEPTION(InvalidOperationException, "Invalid Operation", InstructionException);

extern inline fml_word eval_binop(fml_word, fml_word, fml_word);

fml_word read_argument(fml_word arg, fml_machine *self, fml_addr offs){
  fml_word val = 0;
  switch(arg & LOCATION_MASK){
    case REG_X:
      val = self->x;
      break;
    case REG_Y:
      val = self->y;
      break;
    case CONSTANT:
      val = self->ram->prg_ram[self->pc + offs];
      break;
    case ACC_STACK:
      val = pop(self->s);
      break;
    default:
      fprintf(stderr,"Undefined argument location !?");
      E4C_THROW(FmlException,NULL);
      break;
  }

  if((arg & ADDRESS_MASK) != 0){
    val += (arg & SP_MASK) != 0 ? self->sp : 0;  
    return read(self->ram,val);
  }else{
    return val;
  }
}

void write_argument(fml_word arg, fml_machine *self, fml_addr offs, fml_word val){
  if((arg & ADDRESS_MASK) != 0){
    //We get the address as we should have gotten a normal argument.
    fml_addr addr = read_argument(arg & LOCATION_MASK, self, offs);
    addr += (arg & SP_MASK) != 0 ? self->sp : 0;  
    printf("wiriting to address 0x%lX\n",addr);
    write(self->ram,addr,val);
  }else{
    switch(arg & LOCATION_MASK){
      case REG_X:
        printf("Writing 0x%lx to x\n",val);
        self->x = val;
        break;
      case REG_Y:
        printf("Writing 0x%lx to y\n",val);
        self->y = val;
        break;
      case CONSTANT:
        fprintf(stderr,"Attempting to write to constant.\n");
        E4C_THROW(MoveToConstantException, NULL);
        break;
      case ACC_STACK:
        printf("Writing 0x%lx to s\n",val);
        push(self->s,val);
        break;
      default:
        fprintf(stderr,"Undefined argument location !?\n");
        E4C_THROW(FmlException,NULL);
        break;
    }
  }
}

int eval(fml_machine *self, uint64_t max_steps){
  puts("Entered eval function");
  uint64_t steps = 0;
  while(!self->halt){
    puts("");
    printf("Evaluating at 0x%lx\n",self->pc);
    fml_word instruction = self->ram->prg_ram[self->pc];
    printf("Current instruction is 0x%lX\n",instruction);

    fml_addr step_length = (instruction & STEP_MASK) >> STEP_SHIFT;

    if(instruction != NOP){
      fml_word op_index = (instruction & OPCODE_MASK) >> OPCODE_SHIFT;
      printf("Op index is\t 0x%lx\n",op_index);
      printf("Step len is\t 0x%lx\n",step_length);
      printf("Arg_0 is\t 0x%LX. Arg_1 is 0x%LX\n",
          (instruction & A0_MASK) >> A0_SHIFT,
          (instruction & A1_MASK) >> A1_SHIFT
          );

      if(op_index >= OPERATIONS_END){
        fprintf(stderr,"Operation out of bounds: %lu(%lX)\n",op_index,op_index);
        E4C_THROW(InvalidOperationException,NULL);
        return 1;
      }

      if(op_index < CONTROL_START){
        if(op_index < BINARY_START){
          //Special Operations
            fml_word val = 0;
            val = read_argument((instruction & A0_MASK) >> A0_SHIFT, self, A0_OFFS);
            printf("Read value 0x%lx(%lu)\n",val,val);
            switch(op_index){
              case(INC_VALUE):
                write_argument((instruction & A0_MASK) >> A0_SHIFT, self, A0_OFFS, val + 1);
                break;
              case(DEC_VALUE):
                write_argument((instruction & A0_MASK) >> A0_SHIFT, self, A0_OFFS, val - 1);
                break;
              case(MOV_VALUE):
                write_argument((instruction & A1_MASK) >> A1_SHIFT, self, A1_OFFS, val);
                break;
            }
        
        }else if(op_index < UNARY_START){
          //Binary Operations
          fml_word a0 = read_argument((instruction & A0_MASK) >> A0_SHIFT, self, A0_OFFS);
          fml_word a1 = read_argument((instruction & A1_MASK) >> A1_SHIFT, self, A1_OFFS);
          push(self->s,eval_binop(op_index, a0, a1));
        }else{
          //Unary Operators
          fprintf(stderr,"Not implemented");
          E4C_THROW(NotImplementedException,NULL);
        }
        self->pc += step_length;

      }else{
        //Control operations. 
        //They dont handle the PC like normal people
        switch(op_index){
          case(HLT_VALUE):
            puts("Executing halt");
            self->halt = 1;
            break;
          default:
            fprintf(stderr,"Not implemented");
            E4C_THROW(NotImplementedException,NULL);
            break;
        }
      
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



