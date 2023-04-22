#include <string.h>
#include <stdio.h>
#include "vm_grammar.h"
#define STACK_END 1024
#define INSTR_SET_END 512
#define sp registers[SP]
#define ip registers[IP]
#define GET_INSTR(i) (i >> 28)
#define GET_MODE(i) ((i << 4) >> 28)
#define GET_N_BYTES(src,n) ((i << (sizeof(src)-n)*8) >> (sizeof(src)-n))*8

#define GET_UINT8(i,ind) ((i << ind*8) >> 24)
#define GET_UINT16(i,ind) ((i << ind*16) >> 16)

typedef uint8_t byte;
static uint16_t registers[REG_NUM];
static uint16_t stack[STACK_END];
static instr instr_set[INSTR_SET_END];

void instr_mov(instr i);
void instr_push(instr i);
void instr_pop(instr i);
void instr_add(instr i);
void instr_sub(instr i);
void instr_mul(instr i);
void instr_div(instr i);
void instr_op(instr i);

void initialize()
{
   sp = 0;
   ip = 0;
}

void read(FILE* file)
{
   fread(instr_set,sizeof(instr),INSTR_SET_END,file);
}

instr fetch()
{
   return instr_set[ip++];
}

void run()
{
   while (1)
   {
      instr i = fetch();
      if(i == TERM)
         return;
      switch (GET_INSTR(i))
      {
      case MOV:
         instr_mov(i);
         break;
      case PUSH:
         instr_push(i);
         break;
      case POP:
         instr_pop(i);
         break;
      case ADD:
         instr_add(i);
         break;
      case SUB:
         instr_sub(i);
         break;
      case MUl:
         instr_mul(i);
         break;
      case DIV:
         instr_div(i);
         break;
      case OP:
         instr_op(i);
         break;
      case PRINT:
         printf("%d",registers[GET_UINT8(i,3)]);
         break;
      default:
         printf("invalid instruction");
      }
   }
}

int main()
{
   FILE* file = fopen("in.bin","rb");
   read(file);
   run();
}

void instr_mov(instr i)
{
   uint8_t arg1 = GET_UINT8(i,1);
   uint16_t arg2 = GET_UINT16(i,1);
   switch (GET_MODE(i))
   {
   case 0: //reg to reg
      registers[arg1] = registers[arg2];
      break;
   case 1: //immed to reg
      registers[arg1] = arg2;
      break;
   case 2: //immed to memory
      stack[registers[arg1]] = arg2;
      break;
   case 3: //memory to reg
      registers[arg1] = GET_N_BYTES(stack[registers[GET_UINT8(arg2,0)]],GET_UINT8(arg2,1));
      break;
   case 4: //register to memory
      stack[registers[arg1]] = GET_N_BYTES(registers[GET_UINT8(arg2,0)],GET_UINT8(arg2,1));
      break;
   default:
      break;
   }
}

void instr_push(instr i)
{
   uint16_t v = GET_UINT16(i,1);
   if(GET_MODE(i))//register
   {
      stack[sp++] = registers[v];
   }
   else //immed
   {
      stack[sp++] = v;
   }
}

void instr_pop(instr i)
{
   uint16_t v = GET_UINT16(i,1);
   if(GET_MODE(i))//register
   {
      registers[v] = stack[sp--];
   }
   else //memory
   {
      stack[registers[v]] = stack[sp--];
   }
}

void instr_add(instr i)
{
   if(GET_MODE(i))//register
   {
      registers[GET_UINT8(i,1)] += registers[GET_UINT16(i,1)];
   }
   else//immed
   {
      registers[GET_UINT8(i,1)] += GET_UINT16(i,1);
   }
}

void instr_sub(instr i)
{
   if(GET_MODE(i))//register
   {
      registers[GET_UINT8(i,1)] -= registers[GET_UINT16(i,1)];
   }
   else//immed
   {
      registers[GET_UINT8(i,1)] -= GET_UINT16(i,1);
   }
}

void instr_mul(instr i)
{
   if(GET_MODE(i))//register
   {
      registers[GET_UINT8(i,1)] *= registers[GET_UINT16(i,1)];
   }
   else//immed
   {
      registers[GET_UINT8(i,1)] *= GET_UINT16(i,1);
   }
}

void instr_div(instr i)
{
   if(GET_MODE(i))//register
   {
      registers[GET_UINT8(i,1)] /= registers[GET_UINT16(i,1)];
   }
   else//immed
   {
      registers[GET_UINT8(i,1)] /= GET_UINT16(i,1);
   }
}

void instr_op(instr i)
{
   switch (GET_MODE(i))
   {
   case AND:
      registers[GET_UINT8(i,2)] &= registers[GET_UINT8(i,3)];
      break;
   case OR:
      registers[GET_UINT8(i,2)] |= registers[GET_UINT8(i,3)];
      break;
   case XOR:
      registers[GET_UINT8(i,2)] ^= registers[GET_UINT8(i,3)];
      break;
   default:
      break;
   }
}