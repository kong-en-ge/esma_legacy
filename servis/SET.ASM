	.286p
	.model LARGE graphics
	extrn	_setmem:far
	extrn	@$bdele$qnv:far
	extrn	@$bnew$qui:far

	.code  graphics


start	proc	near
	LES	bx,[bp+6]
	LES	bx,ES:[bx]
	mov	ax,es
	or	ax,bx
	jz	exit
	mov	ax,[bp+10]
	mov	cx,ax
	shr	ax,1
	shr	ax,1
	shr	ax,1
	and	cx,7
	xor	ah,ah
	add	bx,ax
	mov	al,1
	shl	al,cl
	retn
exit:   pop	ax
	mov	ax,1
	pop	bp
	retf	6
start	endp
   ;
   ;	pascal SET::SET(int maxnum) {
   ;
@SET@$bctr$qi	proc	far
	push	bp
	mov	bp,sp
	mov	ax,word ptr [bp+6]
	or	ax,word ptr [bp+8]
	jne	short @1@86
	push	4
	call	@$bnew$qui
	add	sp,2
	mov	word ptr [bp+8],dx
	mov	word ptr [bp+6],ax
	or	ax,dx
	je	short @1@114
@1@86:
   ;
   ;	  init(maxnum);
   ;
	push	word ptr [bp+10]
	push	word ptr [bp+8]
	push	word ptr [bp+6]
	call	far ptr @SET@s_init$qi
@1@114:
   ;
   ;	}
   ;
	mov	dx,word ptr [bp+8]
	mov	ax,word ptr [bp+6]
	pop	bp
	ret	6
@SET@$bctr$qi	endp
   ;
   ;	pascal SET::~SET() {
   ;
@SET@$bdtr$qv	proc	far
	push	bp
	mov	bp,sp
	push	si
	mov	si,word ptr [bp+10]
	mov	ax,word ptr [bp+6]
	or	ax,word ptr [bp+8]
	je	short @2@142
   ;
   ;	  delete base;
   ;
	les	bx,dword ptr [bp+6]
	push	word ptr es:[bx+2]
	push	word ptr es:[bx]
	call	far ptr @$bdele$qnv
	add	sp,4
	test	si,1
	je	short @2@142
	push	word ptr [bp+8]
	push	word ptr [bp+6]
	call	far ptr @$bdele$qnv
	add	sp,4
@2@142:
   ;
   ;	}
   ;
	pop	si
	pop	bp
	ret	6
@SET@$bdtr$qv	endp
   ;
   ;	void pascal SET::init(int maxnum) {
   ;
@SET@s_init$qi	proc	far
	push	bp
	mov	bp,sp
	push	si
	mov	dx,word ptr [bp+10]
   ;
   ;	  int register size;
   ;	  if (maxnum) base=new char[size=(maxnum+7)/8];else base=NULL;
   ;
	or	dx,dx
	je	short @3@86
	mov	ax,dx
	add	ax,7
	mov	bx,8
	cwd
	idiv	bx
	mov	si,ax
	push	ax
	call	far ptr @$bnew$qui
	add	sp,2
	les	bx,dword ptr [bp+6]
	mov	word ptr es:[bx+2],dx
	mov	word ptr es:[bx],ax
	jmp	short @3@114
@3@86:
	les	bx,dword ptr [bp+6]
	mov	word ptr es:[bx+2],0
	mov	word ptr es:[bx],0
@3@114:
   ;
   ;	  if (base) setmem(base,size,0);
   ;
	les	bx,dword ptr [bp+6]
	mov	ax,word ptr es:[bx]
	or	ax,word ptr es:[bx+2]
	je	short @3@170
	push	0
	push	si
	push	word ptr es:[bx+2]
	push	word ptr es:[bx]
	call	far ptr _setmem
	add	sp,8
@3@170:
   ;
   ;	}
   ;
	pop	si
	pop	bp
	ret	6
@SET@s_init$qi	endp
   ;
   ;	void pascal SET::add(int num) {
   ;
@SET@s_add$qi	proc	far
	push	bp
	mov	bp,sp
	call	start
	or	ES:[bx],al
	pop	bp
	ret	6
@SET@s_add$qi	endp
   ;
   ;	void pascal SET::sub(int num) {
   ;
@SET@s_sub$qi	proc	far
	push	bp
	mov	bp,sp
	call	start
	not	al
	and	ES:[bx],al
	pop	bp
	ret	6
@SET@s_sub$qi	endp
   ;
   ;	void pascal SET::xor(int num) {
   ;
@SET@S_xor$qi	proc	far
	push	bp
	mov	bp,sp
	call	start
	xor	ES:[bx],al
	pop	bp
	ret	6
@SET@s_xor$qi	endp
   ;
   ;	void pascal SET::addall(int maxnum) {
   ;
@SET@s_addall$qi	proc	far
	push	bp
	mov	bp,sp
   ;
   ;	  setmem(base,(maxnum+7)/8,0xff);
   ;
	push	0ffh
	mov	ax,word ptr [bp+10]
	add	ax,7
	mov	bx,8
	cwd
	idiv	bx
	push	ax
	les	bx,dword ptr [bp+6]
	push	word ptr es:[bx+2]
	push	word ptr es:[bx]
	call	far ptr _setmem
	add	sp,8
   ;
   ;	}
   ;
	pop	bp
	ret	6
@SET@s_addall$qi	endp
   ;
   ;	int pascal SET::in(int num) {
   ;
@SET@s_in$qi	proc	far
	push	bp
	mov	bp,sp
	call	start
	xor	dx,dx
	and	al,ES:[bx]
	je	short l1
	inc	dx
l1:
	mov	ax,dx
	pop	bp
	ret	6
@SET@s_in$qi	endp

	public	@SET@s_addall$qi
	public	@SET@s_xor$qi
	public	@SET@s_in$qi
	public	@SET@s_sub$qi
	public	@SET@s_add$qi
	public	@SET@s_init$qi
	public	@SET@$bdtr$qv
	public	@SET@$bctr$qi
end
