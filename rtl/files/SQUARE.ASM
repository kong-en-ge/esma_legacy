	.LIST
	.286P
	.MODEL	LARGE _TEXT
	.CODE   _TEXT
	.287
c2	dd	2.0
	EMUL

SQUARE_METR	PROC  PASCAL FAR metr:dword
USES	SI
	les	si,metr
	call	SQUARE_2
	fdiv	dword ptr c2
	fabs
	wait
	ret
SQUARE_METR	ENDP
ORIENT_METR	PROC  PASCAL FAR metr:dword
USES	SI
	les	si,metr
	call	SQUARE_2
	fldz
	fcompp	st[1]
	wait
	xor	bx,bx
	fstsw	ax
	wait
	sahf
	ja	o01
	inc	bx
o01:
	mov	ax,bx
	wait
	ret
ORIENT_METR	ENDP
SQUARE_2	PROC C NEAR
USES	DS
LOCAL	x1,y1:word
	push	es
	pop	ds
	add	si,8
	mov	ax,[si]
	mov	x1,ax
	mov	ax,[si+2]
	mov	y1,ax
	fldz
s_loop:
	cmp	[si+4],0C018h
	je	s_ex
	fild	word ptr [si+4]
	fisub	word ptr [si]
	fild	word ptr [si+2]
	fiadd	word ptr [si+6]
	fmulp	st[1]
	faddp	st[1]
	add	si,4
	jmp	s_loop
s_ex:
	fild	word ptr x1
	fisub	word ptr [si]
	fild	word ptr y1
	fiadd	word ptr [si+2]
	fmulp	st[1]
	faddp   st[1]
	wait
	ret
SQUARE_2	ENDP

PUBLIC  SQUARE_METR
PUBLIC	ORIENT_METR
END