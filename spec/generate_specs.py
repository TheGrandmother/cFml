import json as js

def getHexConstant(n):
    hex_repr = hex(n)
    hex_repr = hex_repr.replace("0x","").zfill(16)
    hex_repr = hex_repr.replace("L","")
    hex_repr = "0x" + hex_repr + "ULL"
    return hex_repr

def uhex(n):
    return hex(n) + "U"

def generateMask(range):
    start = range[0]
    end = range[1]
    bit_val = 2**(end-start+1)-1
    if start == 1:
        return getHexConstant(bit_val)
    else:
        return getHexConstant(bit_val << start-1)

def maskBits(range):
    start = range[0]
    end = range[1]
    bit_val = 2**(end-start+1)-1
    if start == 1:
        return bit_val
    else:
        return bit_val << start -1
    


class InstructionLayout():
    shift_lengths ={}
    masks ={}

    argument_masks = {}
    location_values = {}

    def __init__(self, isa_file):
        json = js.load(open(isa_file,'r'))
        isa_json = json["instruction_layout"]
        self.masks["a0_mask"] = isa_json["a0"] 
        self.masks["a1_mask"] = isa_json["a1"]
        self.masks["step_mask"] = isa_json["step_length"]
        self.masks["opcode_mask"] = [isa_json["opcode_start"], 64]
        self.shift_lengths["a0_shift"] = isa_json["a0"][0] - 1
        self.shift_lengths["a1_shift"] = isa_json["a1"][0] - 1
        self.shift_lengths["step_shift"] = isa_json["step_length"][0] - 1
        self.shift_lengths["opcode_shift"] = isa_json["opcode_start"] - 1
        
        #Argument structure
        arg_json = json["argument_structure"]
        self.argument_masks["location_mask"] = arg_json["location_bits"]
        self.argument_masks["address_mask"] = arg_json["address_bit"]
        self.argument_masks["sp_mask"] = arg_json["sp_bit"]
        self.location_values = arg_json["location_values"]

    def printConstants(self):
        print "ISA bitmasks"
        for key, value in self.masks.iteritems():
            print key  + ":\t" + (generateMask(value))

        print "\nISA Shift Values"
        for key, value in self.shift_lengths.iteritems():
            print key  + ":\t" + (uhex(value))

        print "\nArgument Masks"
        for key, value in self.argument_masks.iteritems():
            print key  + ":\t" + (generateMask(value))
            
        print "\nLocation Values"
        for key, value in self.location_values.iteritems():
            print key  + ":\t" + (uhex(value))

    def generateCHeader(self):
        top_level_comment = "/**\n" 
        top_level_comment +="* This file contains the numerical constants for the\n"
        top_level_comment +="* instructions set architecture\n" 
        top_level_comment +="* \n"
        top_level_comment +="* This file was automatically generated.\n"
        top_level_comment +="* DO NOT EDIT THIS FILE DIRECTLY!\n"
        top_level_comment +="**/\n\n"

        include_guard = "#ifndef _ISA_CONSTANTS_H\n#define _ISA_CONSTANTS_H\n\n"

        isa_comment = "//The following constnts concern the instruction layout\n"
        isa_constants = ""

        for key, value in self.masks.iteritems():
            isa_constants += "#define " + key.upper()  + " \t" + (generateMask(value)) + "\n"

        for key, value in self.shift_lengths.iteritems():
            isa_constants += "#define " + key.upper()  + " \t" + (uhex(value)) + "\n"
        isa_constants += "\n\n"

        argument_constants = "//These constants define the strucuture of the argument\n"

        for key, value in self.argument_masks.iteritems():
            argument_constants += "#define " + key.upper()  + " \t" + (generateMask(value)) + "\n"

        for key, value in self.location_values.iteritems():
            argument_constants += "#define " + key.upper()  + " \t" + (uhex(value)) + "\n"

        argument_constants += "\n\n"

        return top_level_comment + include_guard + isa_comment + isa_constants + argument_constants + "#endif"

if __name__ == "__main__":
    print "Starting to generate constants"
    instruction_layout = InstructionLayout("ISA.json")
    print instruction_layout.generateCHeader()

