#ifndef VM_GRAMMAR
#include <stdint.h>
#define VM
#define TERM UINT32_MAX

typedef uint32_t instr;

enum {A,B,C,D,SP,IP,REG_NUM};
enum {MOV,PUSH,POP,ADD,SUB,MUl,DIV,OP,PRINT};
enum {AND,OR,XOR};

#endif

/*
   Instruction layout

   MOV
   [instruction]:4
   [mode]:4
   [arg1]:8
   [arg2]:16

   PUSH
   [instruction]:4
   [mode]:4
   [padding]:8
   [arg]:16

   POP
   [instruction]:4
   [mode]:4
   [padding]:8
   [arg]:16

   ADD
   [instruction]:4
   [mode]:4
   [arg1]:8
   [arg]:16

   SUB
   [instruction]:4
   [mode]:4
   [arg1]:8
   [arg]:16

   MUL
   [instruction]:4
   [mode]:4
   [arg1]:8
   [arg]:16

   DIV
   [instruction]:4
   [mode]:4
   [arg1]:8
   [arg]:16

   OP
   [instruction]:4
   [op]:4 (and or xor)
   [padding]:8
   [arg1]:8
   [arg2]:8

   TERM
   [1]:32

   Instruction documentation

   MOV
   mode 0: register to register copy
   arg 1: registers
   arg 2: registers

   mode 1: immed to register load
   arg 1: register
   arg 2: immed value

   mode 2: immed to memory load
   arg 1: register with addr
   arg 2: immed value

   mode 3: memory to register load
   arg 1: register
   arg 2: [register with addr] : 8 and [copy len] : 8

   mode 4: register to memory load
   arg 1: register with addr
   arg 2: [register] : 8 and [copy len] : 8

   PUSH
   mode 0: immed
   arg: immed value

   mode 1: register
   arg: register

   POP
   mode 0: memory
   arg: register with addr

   mode 1: register
   arg: register

   ADD SUB MUL DIV
   mode 0: immed
   arg 1: register
   arg 2: immed value

   mode 1: register
   arg 1: register
   arg 2: register

   OP
   op:
      and:0 
      or:1
      xor:2
   arg1: register
   arg2:register

   TERM
   The OP instructions padding values prevent it to be equal to 2^32-1
*/