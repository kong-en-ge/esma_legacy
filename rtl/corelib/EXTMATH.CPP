int sign(int value)
{
  return value < 0 ? -1 : value > 0 ? 1 : 0;
}

int sign(double value)
{
  return value < 0 ? -1 : value > 0 ? 1 : 0;
}

#pragma warn -rvl

long longmul(int m1, int m2)
{
  asm {
    mov ax, m1
    imul m2
  }
}

unsigned long ulongmul(unsigned m1, unsigned m2)
{
  asm {
    mov ax, m1
    mul m2
  }
}

double length(int deltaX, int deltaY)
{
  asm {
    FILD word ptr deltaX
    FMUL st, st
    FILD word ptr deltaY
    FMUL st, st
    FADD
    FSQRT
  }
}

double length(double deltaX, double deltaY)
{
  asm {
    fld qword ptr deltaX
    fmul st, st
    fld qword ptr deltaY
    fmul st, st
    fadd
    fsqrt
  }
}

#pragma warn +rvl
