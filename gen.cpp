#include "codegen.hpp"

int main()
{
   Codegen c("in.bin");
   Register r = Register::allocate();
   c.mov(r,Immediate(10));
   c.print(r);
}