#pragma inline
long pascal longmul(int m1,int m2) {
asm	mov	ax,m1
asm	imul	m2
}

unsigned long pascal ulongmul(unsigned int m1,unsigned int m2) {
asm	mov	ax,m1
asm	mul	m2
}

double pascal length(int m1,int m2) {
asm {
	FILD	word ptr m1
	FMUL	st,st
	FILD	word ptr m2
	FMUL 	st,st
	FADD
	FSQRT
}
}

double pascal length(double m1,double m2) {

asm {
	fld	qword ptr m1
	fmul	st,st
	fld	qword ptr m2
	fmul 	st,st
	fadd
	fsqrt
}
}
