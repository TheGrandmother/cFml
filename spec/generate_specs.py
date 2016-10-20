import json as js
import sys
import operator

def makeCDefine(name,value):
    return "#define " + name + "\t" + value + "\n"

def getHexConstant(n):
    hex_repr = hex(n)
    hex_repr = hex_repr.replace("0x","").zfill(16)
    hex_repr = hex_repr.replace("L","")
    hex_repr = "0x" + hex_repr + "ULL"
    return hex_repr

def uhex(n):
    return hex(n) + "ULL"

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
    
class OperationsLayout():
    basic_ops = {}
    binary_ops = {}
    unary_operations = {}
    contron_instructions = {}
    values = {}
    binary_start = 0
    unary_start = 0
    control_start = 0
    operations_end = 0
    basic_ops   = None
    binary_ops  = None
    unary_ops   = None
    control_ops = None
    isa_object = None

    argument_counts = {}

    def __init__(self, isa_object, instruction_file):
        json = js.load(open(instruction_file,'r'))
        op_json = json
        self.isa_object = isa_object
        self.basic_ops   = op_json["Basic_Ops"]
        self.binary_ops  = op_json["Binary_Ops"]
        self.unary_ops   = op_json["Unary_Ops"]
        self.control_ops = op_json["Control_Ops"]

        self.buildValueMap()
        self.makeArgumentCounts()
        
    def makeArgumentCounts(self):
        for key, value in self.basic_ops.iteritems():
            self.argument_counts.update({key : self.basic_ops[key]["args"]})
        for key, value in self.control_ops.iteritems():
            self.argument_counts.update({key : self.control_ops[key]["args"]})

    def buildValueMap(self):
        index = 1 #index 0 is reserved for NOP
        for elem in self.basic_ops:
            self.values.update({self.basic_ops[elem]["mnemonic"] : index})
            index = index +1
        self.binary_start = index 

        for elem in self.binary_ops:
            self.values.update({self.binary_ops[elem]["mnemonic"] : index})
            index = index +1
        self.unary_start = index

        for elem in self.unary_ops:
            self.values.update({self.unary_ops[elem]["mnemonic"] : index})
            index = index +1
        self.control_start = index
        
        for elem in self.control_ops:
            self.values.update({self.control_ops[elem]["mnemonic"] : index})
            index = index +1
        self.operations_end = index;

    def makeBinopFile(self):
        sorted_mne = sorted(self.values.items(), key = operator.itemgetter(1))
        sorted_mne = sorted_mne[self.binary_start-1:self.unary_start-1]

        def makeCase(op):
            key = "\tcase(" + op["mnemonic"] + "_VALUE):\n";
            operation = "ret = " + op["operation"] + ";\n"
            if "check" in op:
                body =  "\t\tif(" + op["check"] + "){\n"
                body += "\t\t\t" + operation
                body += "\t\t}else{\n"
                body += "\t\t\t" + op["fail_action"] + ";\n"
                body += "\t\t}\n"
            else:
                body = "\t\t" + operation
            body += "\t\tbreak;\n"
            return key + body

        switch_body = "\tswitch(op_index){\n"
        for (men,val) in sorted_mne:
            switch_body += makeCase(self.binary_ops[men]) + "\n"
        switch_body += "\t}\n"
        function  = "inline fml_word eval_binop(fml_word op_index,fml_word a0, fml_word a1){\n"
        function += "\tfml_word ret = 0;\n\n"
        function += switch_body + "\n"
        function += "\treturn ret;\n}\n"
        header_comment  = "/**\n* Thise file contains auto generated code for the\n"
        header_comment += "* eval function.\n* This file is generated from instructions.json.\n"
        header_comment += "*\n* DO NOT EDIT THIS FILE\n**/\n\n"
        includes_n_guards  = "#include \"../types.h\"\n"
        includes_n_guards += "#include \"../exceptions.h\"\n"
        includes_n_guards += "#include \"../e4c/e4c_lite.h\"\n"
        includes_n_guards += "#include \"../constants/isa_constants.h\"\n"
        includes_n_guards += "#include \"../constants/operation_constants.h\"\n\n"
        includes_n_guards += "#include <stdio.h>\n\n"
        includes_n_guards += "#ifndef _BINOP_EVAL_H\n#define _BINOP_EVAL_H\n\n"
        return header_comment + includes_n_guards + function + "\n#endif\n"

    def makeValueDefines(self):
        c_defines = ""
        for key, value in self.values.iteritems():
            c_defines += makeCDefine(key.upper()+"_VALUE",uhex(value))
        return c_defines

    def makeReverseLookupTable(self):
        sorted_mne = sorted(self.values.items(), key = operator.itemgetter(1))
        def makeCase(mnem):
            key = "\tcase(" + mnem.upper() + "_VALUE):\n";
            body = "\t\t return \"" + mnem + "\";\n"
            return key + body

        switch_body = "\tswitch(op_index){\n"
        for (men,val) in sorted_mne:
            switch_body += makeCase(men) + "\n"
        switch_body += "\t}\n"
        function  = "inline char *mne_lookup(fml_word op_index){\n"
        function += switch_body + "\n"
        function += "}\n"
        header_comment  = "/**\n* Thise file contains auto generated code for \n"
        header_comment += "* reverse mnemonic lookup.\n* This file is generated from instructions.json.\n"
        header_comment += "*\n* DO NOT EDIT THIS FILE\n**/\n\n"
        includes_n_guards  = "#include \"../types.h\"\n"
        includes_n_guards += "#include \"../src/constants/isa_constants.h\"\n"
        includes_n_guards += "#include \"../src/constants/operation_constants.h\"\n"
        includes_n_guards += "#ifndef _MNE_LOOKUP_H\n#define _MNE_LOOKUP_H\n\n"
        return header_comment + includes_n_guards + function + "\n#endif\n"
    
    def makeArgumentCountDefines(self):
        c_defines = ""
        for key, value in self.argument_counts.iteritems():
            c_defines += makeCDefine(key.upper()+"_ARG_COUNT",uhex(value))
        return c_defines
    
    def makeHintDefines(self):
        c_defines = ""
        c_defines += makeCDefine("BINARY_START",uhex(self.binary_start))
        c_defines += makeCDefine("UNARY_START",uhex(self.unary_start))
        c_defines += makeCDefine("CONTROL_START",uhex(self.control_start))
        c_defines += makeCDefine("OPERATIONS_END",uhex(self.operations_end))
        return c_defines

    def makeHeaderFile(self):
        header_text = "";
        header_text += "/**\n"
        header_text += "* This file contains the constats for all the instructions\n"
        header_text += "* \n * The file was auto generated. DO NOT EDIT!\n**/\n\n"
        header_text += "#ifndef _OPERATION_CONSTANTS_H\n"
        header_text += "#define _OPERATION_CONSTANTS_H\n\n"
        header_text += "#define NOP\t0\n\n"
        header_text += "//These are instruction hints\n"
        header_text += "//Used to for performance\n"
        header_text += self.makeHintDefines()
        header_text += "\n//These are the argument counts for the\n"
        header_text += "//special and control operations\n"
        header_text += self.makeArgumentCountDefines()
        header_text += "\n//These are the numeric values of the operations\n"
        header_text += self.makeValueDefines()
        header_text += "\n#endif\n"

        return header_text

    def writeHeaderFile(self):
        header_file = open("operation_constants.h",'w')
        header_file.write(self.makeHeaderFile())
        header_file.close()

    def writeCodeFile(self):
        header_file = open("binop_eval.h",'w')
        header_file.write(self.makeBinopFile())
        header_file.close()

    def writeReverseLookupFile(self):
        header_file = open("mne_lookup.h",'w')
        header_file.write(self.makeReverseLookupTable())
        header_file.close()

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

    def makeHeaderFile(self):
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
        isa_constants += makeCDefine("a0_offs".upper(),uhex(1));
        isa_constants += makeCDefine("a1_offs".upper(),uhex(2));
        isa_constants += "\n\n"

        argument_constants = "//These constants define the strucuture of the argument\n"

        for key, value in self.argument_masks.iteritems():
            argument_constants += "#define " + key.upper()  + " \t" + (generateMask(value)) + "\n"

        for key, value in self.location_values.iteritems():
            argument_constants += "#define " + key.upper()  + " \t" + (uhex(value)) + "\n"

        argument_constants += "\n\n"

        header_text =  top_level_comment + include_guard + isa_comment + isa_constants + argument_constants + "#endif\n"
        return header_text

    def writeHeaderFile(self):
        header_file = open("isa_constants.h",'w')
        header_file.write(self.makeHeaderFile())
        header_file.close()

if __name__ == "__main__":
    print_files = False
    write_files = False
    make_instructions = False
    make_operations = False
    make_code = False
    make_lookup = False
    for arg in sys.argv:
        if arg == "-i":
            make_instructions = True;
            continue
        elif arg == "-o":
            make_operations = True;
            continue
        elif arg == "-c":
            make_code = True;
            continue
        elif arg == "-rl":
            make_lookup = True;
            continue
        elif arg == "-p":
            print_files = True
            continue
        elif arg == "-w":
            write_files = True
            continue
        elif arg == sys.argv[0]:
            continue #this is just the file name
        else:
            print "Invalid argument: " + arg
            exit(1)


    instructions = InstructionLayout("ISA.json")
    operations = OperationsLayout(instructions, "instructions.json")
    if make_operations:
        if print_files:
            print operations.makeHeaderFile()
        if write_files:
            operations.writeHeaderFile()

    if make_instructions:
        if print_files:
            print instructions.makeHeaderFile()
        if write_files:
            instructions.writeHeaderFile()

    if make_code:
        if print_files:
            print operations.makeBinopFile()
        if write_files:
            operations.writeCodeFile()

    if make_lookup:
        if print_files:
            print operations.makeReverseLookupTable()
        if write_files:
            operations.writeReverseLookupFile()

