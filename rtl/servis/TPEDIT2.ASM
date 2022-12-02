	.286
	.model LARGE graphics
	extrn SETPALETTE:far
	.DATA
	extrn _COL1:word
	extrn _COL2:word
	extrn _NCOL:word
	extrn _SYNC:byte
iscol1 label byte
	db 0
	.code graphics
int08n label byte
	db 12
int8 proc far
	pusha
	push ds
	push es
	cli
	mov	ax,DGROUP
	mov	ds,ax
	cmp     byte ptr _SYNC,0
	jnz     cl0
	dec	cs:byte ptr int08n
	jnz	cl0
	mov	byte ptr _SYNC,1
	db   9ah
wrc  label dword
	dd  ?
	mov	cs:byte ptr int08n,12
	mov	byte ptr _SYNC,0
cl0:
	pop es
	pop ds
	popa
	db 0eah
int8v2 label dword
	dd 0
int8 endp

setvec8 proc far   ;на входе адрес функции мигалки
i8 equ [bp+6]
	push bp
	mov  bp,sp
	push	ds
	mov  ax,351Ch
	int  21h
	mov  dx,bx
	mov  bx,offset int8v2
	mov  ax,cs:[bx]
	or   ax,cs:[bx+2]
	jnz  s8ex	
	mov  cs:[bx],dx
	mov  cs:[bx+2],es
	lds	dx,i8
	mov  bx,offset wrc
	mov  cs:[bx],dx
	mov  cs:[bx+2],ds
	mov  byte ptr cs:int08n,12
	mov  dx,cs
	mov  ds,dx
	mov  dx,offset int8
	mov	ax,251Ch
	int	21h
s8ex:
	pop	ds
	pop  bp
	ret  4
setvec8 endp
restore8 proc far
	push  ds
	mov   bx,offset int8v2
	lds   dx,cs:[bx]
	mov   ax,dx
	or    ax,cs:[bx+2]
	jz    r8ex	 
	mov   ax,251Ch
	int   21h
	xor   ax,ax
	mov   cs:[bx],ax
	mov   cs:[bx+2],ax		
	mov   ax,1	
r8ex:
	pop   ds
	ret
restore8 endp
twinkle proc far
	pusha
	push	ds
	mov	ax,DGROUP
	mov	ds,ax
	not	byte ptr iscol1
	mov	 ax,_COL2
	cmp	byte ptr iscol1,0
	jnz	tl1
	mov	 ax, _COL1
tl1:
	call	SETPALETTE pascal,_NCOL,ax
	pop	ds
	popa
	ret
twinkle endp
  public SETVEC8
  public TWINKLE
  public RESTORE8
  public ISCOL1
end