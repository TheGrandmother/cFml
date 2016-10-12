import json as js

class InstructionLayout():
    a0_mask = 0
    a1_mask = 0
    a0_shift = 0
    a1_shift = 0
    step_length_mask = 0
    step_length_shift = 0
    instruction_mask = 0
    instruction_shift = 0

if __name__ == "__main__":
    print "Starting to generate constants"
    isa_file = open("ISA.json",'r')
    isa_json =  js.load(isa_file)
    print isa_json["Instruction_layout"]
