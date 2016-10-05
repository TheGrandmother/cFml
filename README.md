# cFml
C implementation of the FML machine

##Ideas

Here goes general ideas for improvements

###Type information included in instruction
Using a 64bit word length this gives us a lot of extra information 
to put into the instructions.

We have enoug space to add two 4 bit fields each corresponding to
the type of one argument. This would make things really really nice.

This would enable nice handling of instructions that would operate
differently for different types and eliminate the need to add different
instructions for different types. This would make implementing floats
much easier.

The assembler could handle the type information automatically.


