#include "e4c/e4c_lite.h"
#include "constants/isa_constants.h"
#include "constants/operation_constants.h"
#include "components.h"
#include "stack.h"
#include "ram.h"
#include <stdio.h>


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
      //This should not happen.
      //TODO: Throw exception
      fprintf(stderr,"Undefined argument location !?");
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
    printf("wiriting to address 0x%LX\n",addr);
    write(self->ram,addr,val);
  }else{
    switch(arg & LOCATION_MASK){
      case REG_X:
        self->x = val;
        break;
      case REG_Y:
        self->y = val;
        break;
      case CONSTANT:
        //Not allowed!!!!
        //TODO: Throw exception
        fprintf(stderr,"Attempting to write to constant.\n");
        exit(1);
        break;
      case ACC_STACK:
        push(self->s,val);
        break;
      default:
        //This should not happen.
        //TODO: Throw exception
        fprintf(stderr,"Undefined argument location !?\n");
        break;
    }
  }
}

int eval(fml_machine *self, uint64_t max_steps){
  puts("Entered eval function");
  uint64_t steps = 0;
  while(!self->halt){
    puts("");
    printf("Evaluating at 0x%LX\n",self->pc);
    fml_word instruction = self->ram->prg_ram[self->pc];
    printf("Current instruction is 0x%X\n",instruction);

    fml_addr step_length = (instruction & STEP_MASK) >> STEP_SHIFT;

    if(instruction != NOP){
      fml_word op_index = (instruction & OPCODE_MASK) >> OPCODE_SHIFT;
      printf("Op index is\t 0x%LX\n",op_index);
      printf("Step len is\t 0x%LX\n",op_index);
      printf("Arg_0 is\t 0x%LX. Arg_1 is 0x%LX\n",
          (instruction & A0_MASK) >> A0_SHIFT,
          (instruction & A1_MASK) >> A1_SHIFT
          );

      if(op_index >= OPERATIONS_END){
        //Invalid operation
        //TODO: Throw exception
        fprintf(stderr,"Operation out of bounds: %lu(%lX)\n",op_index,op_index);
        return 1;
      }

      if(op_index < CONTROL_START){
        puts("Not a control operation.");
        if(op_index < BINARY_START){
          //Special Operations
            fml_word val = 0;
            val = read_argument((instruction & A0_MASK) >> A0_SHIFT, self, A0_OFFS);
            printf("Read value 0x%LX(%lu)\n",val,val);
          switch(op_index){
          case(INC_VALUE):
            puts("Executing INC");
            write_argument((instruction & A0_MASK) >> A0_SHIFT, self, A0_OFFS, val + 1);
            break;
          case(DEC_VALUE):
            puts("Executing DEC");
            write_argument((instruction & A0_MASK) >> A0_SHIFT, self, A0_OFFS, val - 1);
            break;
          case(MOV_VALUE):
            puts("Executing MOV");
            write_argument((instruction & A1_MASK) >> A1_SHIFT, self, A1_OFFS, val);
            break;
          }
        
        }else if(op_index < UNARY_START){
          //Binary Operations
          fprintf(stderr,"Not implemented");
        }else{
          //Unary Operators
          fprintf(stderr,"Not implemented");
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



