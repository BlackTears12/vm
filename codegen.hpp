#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <bitset>
#include "vm_grammar.h"

class Value{};
class Register : public Value
{
   static bool registers[REG_NUM];
   uint8_t regIndex;
   Register(uint8_t i){regIndex = i;}
public:
   static void init(){ memset(registers,0,REG_NUM); }
   uint16_t getValue(){ return regIndex; }

   static Register allocate()
   {
      int i = 0;
      while(i < REG_NUM && !registers[i]){
         ++i;
      }
      registers[i] = true;
      return Register(i);
   }

   static void free(Register r)
   {
      registers[r.regIndex] = false;
   }
};

class Immediate : public Value
{
   uint16_t value;
public:
   Immediate(uint16_t v){value = v;}
   uint16_t getValue(){ return value; }
};

class MemAddr : public Value
{
   Register reg;
public:
   MemAddr(Register r):reg(r){}
   uint16_t getValue(){ return reg.getValue(); }
};

class Codegen
{
   std::ofstream out;

   instr constructInstr(uint8_t code,uint8_t mode,uint8_t arg1,uint16_t arg2)
   {
      uint8_t headr = (code << 4) | mode;
      return construct(construct(headr,arg1),arg2);
   }

   uint16_t construct(uint8_t a,uint8_t b){
      return (((uint16_t)a) << 8) | b;
   }

   uint32_t construct(uint16_t a, uint16_t b){
      return (((uint32_t)a) << 16) | b;
   }

   void write(instr i){
      out.write((char*)&i,sizeof(instr));
   }

   void write(uint8_t code,uint8_t mode,uint8_t arg1,uint16_t arg2){
      write(constructInstr(code,mode,arg1,arg2));
   }

public:
   Codegen(std::string s) : out(s,std::ios::out | std::ios::binary){
      Register::init();
   }

   ~Codegen(){
      instr i(TERM);
      out.write((char*)&i,sizeof(instr));
      out.close();
   }

   void mov(Register r1,Register r2){
      write(MOV,0,r1.getValue(),r2.getValue());
   }

   void mov(Register r,Immediate im){
      write(MOV,1,r.getValue(),im.getValue());
   }

   void mov(Immediate im,MemAddr m){
      write(MOV,2,im.getValue(),m.getValue());
   }

   void mov(Register r,MemAddr m){
      write(MOV,3,r.getValue(),m.getValue());
   }

   void mov(MemAddr m,Register r){
      write(MOV,4,m.getValue(),r.getValue());
   }

   void push(Immediate i){
      write(PUSH,0,0,i.getValue());
   }

   void push(Register r){
      write(PUSH,1,0,r.getValue());
   }

   void pop(MemAddr m){
      write(POP,0,0,m.getValue());
   }

   void pop(Register r){
      write(POP,1,0,r.getValue());
   }

   void binary_op(uint16_t op,Register r,Immediate m){
      write(op,0,r.getValue(),m.getValue());
   }

   void binary_op(uint16_t op,Register r1,Register r2){
      write(op,1,r1.getValue(),r2.getValue());
   }

   void logical_op(uint8_t op,Register r1,Register r2){
      write(OP,op,r1.getValue(),r2.getValue());
   }  

   void print(Register r){
      write(PRINT,0,0,r.getValue());
   }

};
bool Register::registers[REG_NUM];