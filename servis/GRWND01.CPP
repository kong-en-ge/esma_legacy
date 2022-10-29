#pragma inline
#include "grwnd.hpp"

int pascal xyinregion(int x,int y,REGION *reg) {
asm	push	ds
asm 	lds	bx,reg
asm	mov	ax,x
asm	cmp	ax,[bx]
asm     jl	l00
asm	cmp	ax,[bx+4]
asm	jg	l00
asm	mov	ax,y
asm	cmp	ax,[bx+2]
asm	jl	l00
asm	cmp	ax,[bx+6]
asm     jg	l00
asm     mov	ax,1
asm	jmp	l01
l00:
asm	xor	ax,ax
l01:
asm	pop	ds
	return _AX;
}
int pascal regionregion(REGION *reg1,REGION *reg2) {
asm	push	ds
asm	lds	si,reg1
asm	les	bx,reg2
asm	mov	ax,[si]
asm	cmp	ax,es:[bx+4]
asm	jg	l00
asm	mov	ax,[si+4]
asm	cmp	ax,es:[bx]
asm	jl	l00
asm	mov	ax,[si+2]
asm	cmp	ax,es:[bx+6]
asm	jg	l00
asm	mov	ax,[si+6]
asm	cmp	ax,es:[bx+2]
asm	jl	l00
asm	mov	ax,1
asm	jmp	l01
l00:
asm	xor	ax,ax
l01:
asm     pop	ds
	return 	_AX;
}

int pascal xydregion(int x,int y,int delta,REGION *reg) {
REGION r;
asm     mov  	ax,x
asm	mov	bx,ax
asm	add	ax,delta
asm	jno	l00
asm	mov	ax,0x7fff
l00:
asm 	mov	[r.right],ax
asm     sub	bx,delta
asm	mov	[r.left],bx
asm	mov	ax,y
asm	mov	bx,ax
asm	add	ax,delta
asm	jno	l01
asm	mov	ax,0x7fff
l01:
asm  	mov	[r.bot],ax
asm	sub	bx,delta
asm	mov	[r.top],bx
	return  regionregion(&r,reg);
}