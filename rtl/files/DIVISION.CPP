#pragma inline

long longmul(int m1, int m2)
{
  asm mov ax, m1
  asm imul m2
}

unsigned long ulongmul(unsigned int m1, unsigned int m2)
{
  asm mov ax, m1
  asm mul m2
}

double length(int m1, int m2)
{
  asm FILD	word ptr m1
	asm FMUL	st, st
	asm FILD	word ptr m2
	asm FMUL 	st, st
	asm FADD
	asm FSQRT
}

double length(double m1, double m2)
{
  asm	fld	qword ptr m1
	asm fmul	st, st
  asm fld	qword ptr m2
	asm fmul 	st, st
	asm fadd
  asm fsqrt
}
