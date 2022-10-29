	  .286
	  .model LARGE graphics
	extrn	_bioskey:far
	.DATA
  extrn __dec:  word
  extrn _MOUSE_READY :word

_BSS	segment word public 'BSS'
  extrn _X_MIN :	word
  extrn _X_MAX :	word
  extrn _Y_MIN :	word
  extrn _Y_MAX :	word
_BSS	ends

	.CODE graphics
;int pascal mouse_key_status(int *x,int *y){
@MOUSE_KEY_STATUS$QNIT1	proc	far
	enter	8,0
	push	si
   ;
   ;	  int status=0,status2,xcoor,ycoor;
   ;
	mov	word ptr [bp-2],0
@10@50:
   ;
   ;	label0:
   ;	  while (1) {
   ;	    if (status2=bioskey(1)) break;
   ;
	push	1
	call	far ptr _bioskey
	pop	cx
	mov	word ptr [bp-4],ax
	or	ax,ax
	je	short @10@98
	jmp	short @10@194
@10@98:
   ;
   ;	    position_status(&status,x,y);
   ;
	push	ss
	lea	ax,word ptr [bp-2]
	push	ax
	push	word ptr [bp+12]
	push	word ptr [bp+10]
	push	word ptr [bp+8]
	push	word ptr [bp+6]
	push	cs
	call	near ptr @POSITION_STATUS$QNIT1T1
   ;
   ;	    if (status&0x3) break;
   ;
	test	word ptr [bp-2],3
	je	short @10@146
	jmp	short @10@194
@10@146:
   ;
   ;	    return 0;
   ;
	xor	ax,ax
	jmp	@10@1130
	jmp	short @10@50
@10@194:
   ;
   ;	  };
   ;	  if (status) {
   ;
	mov	ax,word ptr [bp-2]
	or	ax,ax
	jz	short @10@434
@10@218:
   ;
   ;	   do {position_status(&status2,&xcoor,&ycoor);} while (status2);
   ;
	push	ss
	lea	ax,word ptr [bp-4]
	push	ax
	push	ss
	lea	ax,word ptr [bp-6]
	push	ax
	push	ss
	lea	ax,word ptr [bp-8]
	push	ax
	push	cs
	call	near ptr @POSITION_STATUS$QNIT1T1
	cmp	word ptr [bp-4],0
	jne	short @10@218
   ;
   ;	   return (status & 1) ? 1 : -1;
   ;
	mov	ax,1
	test	[bp-2],1
	jnz	@10@310
	neg	ax
@10@310:
	jmp	@10@1130
@10@434:
   ;
   ;	  } else
   ;	  if (status2) {
   ;
	cmp	word ptr [bp-4],0
	jne	@@0
	jmp	@10@1106
@@0:
	jmp	short @10@506
@10@482:
   ;
   ;	    while (bioskey(1)) {bioskey(0);};
   ;
	push	0
	call	far ptr _bioskey
	pop	cx
@10@506:
	push	1
	call	far ptr _bioskey
	pop	cx
	or	ax,ax
	jne	short @10@482
   ;
   ;	    int RSC=CallRScommand(&status2);
   ;
	push	ss
	lea	ax,word ptr [bp-4]
	push	ax
	call	far ptr @CALLRSCOMMAND$QNI
	mov	si,ax
   ;
   ;	    if (RSC>RScpgdn)
   ;
	cmp	si,16
	jle	short @10@578
   ;
   ;		 RSC=RSleft;
   ;
	mov	si,6
@10@578:
   ;
   ;	    position_status(&status,&xcoor,&ycoor);
   ;
	push	ss
	lea	ax,word ptr [bp-2]
	push	ax
	push	ss
	lea	ax,word ptr [bp-6]
	push	ax
	push	ss
	lea	ax,word ptr [bp-8]
	push	ax
	push	cs
	call	near ptr @POSITION_STATUS$QNIT1T1
   ;
   ;	    if (RSC==RSquit) {
   ;
	cmp	si,3
	jne	short @10@650
   ;
   ;		 return -1;
   ;
	mov	ax,65535
	jmp	@10@1130
	jmp	@10@1082
@10@650:
   ;
   ;	    } else
   ;	    if (RSC==RSenter) {
   ;
	cmp	si,2
	jne	short @10@722
   ;
   ;		 *x=xcoor;
   ;
	les	bx,dword ptr [bp+10]
	mov	ax,word ptr [bp-6]
	mov	word ptr es:[bx],ax
   ;
   ;		 *y=ycoor;
   ;
	les	bx,dword ptr [bp+6]
	mov	ax,word ptr [bp-8]
	mov	word ptr es:[bx],ax
   ;
   ;		 return 1;
   ;
	mov	ax,1
	jmp	@10@1130
	jmp	short @10@1082
@10@722:
   ;
   ;	    } else
   ;	    if (RSC==RSleft) {
   ;
	cmp	si,6
	jne	short @10@818
   ;
   ;		 xcoor-=_dec;
   ;
	mov	ax,word ptr DGROUP:__dec
	sub	word ptr [bp-6],ax
   ;
   ;		 if (xcoor<X_MIN) xcoor=X_MIN;
   ;
	mov	ax,word ptr [bp-6]
	cmp	ax,word ptr DGROUP:_X_MIN
	jge	short @10@794
	mov	ax,word ptr DGROUP:_X_MIN
	mov	word ptr [bp-6],ax
@10@794:
	jmp	short @10@1082
@10@818:
   ;
   ;	    } else
   ;	    if (RSC==RSright) {
   ;
	cmp	si,7
	jne	short @10@914
   ;
   ;		 xcoor+=_dec;
   ;
	mov	ax,word ptr DGROUP:__dec
	add	word ptr [bp-6],ax
   ;
   ;		 if (xcoor>X_MAX) xcoor=X_MAX;
   ;
	mov	ax,word ptr [bp-6]
	cmp	ax,word ptr DGROUP:_X_MAX
	jle	short @10@890
	mov	ax,word ptr DGROUP:_X_MAX
	mov	word ptr [bp-6],ax
@10@890:
	jmp	short @10@1082
@10@914:
   ;
   ;	    } else
   ;	    if (RSC==RSup) {
   ;
	cmp	si,11
	jne	short @10@1010
   ;
   ;		 ycoor-=_dec;
   ;
	mov	ax,word ptr DGROUP:__dec
	sub	word ptr [bp-8],ax
   ;
   ;		 if (ycoor<Y_MIN) ycoor=Y_MIN;
   ;
	mov	ax,word ptr [bp-8]
	cmp	ax,word ptr DGROUP:_Y_MIN
	jge	short @10@986
	mov	ax,word ptr DGROUP:_Y_MIN
	mov	word ptr [bp-8],ax
@10@986:
	jmp	short @10@1082
@10@1010:
   ;
   ;	    } else
   ;	    if (RSC==RSdn) {
   ;
	cmp	si,12
	jne	short @10@1082
   ;
   ;		ycoor+=_dec;
   ;
	mov	ax,word ptr DGROUP:__dec
	add	word ptr [bp-8],ax
   ;
   ;		if (ycoor>Y_MAX) ycoor=Y_MAX;
   ;
	mov	ax,word ptr [bp-8]
	cmp	ax,word ptr DGROUP:_Y_MAX
	jle	short @10@1082
	mov	ax,word ptr DGROUP:_Y_MAX
	mov	word ptr [bp-8],ax
@10@1082:
   ;
   ;	    };
   ;	    set_position(xcoor,ycoor);
   ;
	push	word ptr [bp-6]
	push	word ptr [bp-8]
	push	cs
	call	near ptr @SET_POSITION$QII
@10@1106:
   ;
   ;	  };
   ;	  goto label0;
   ;
	jmp	@10@50
@10@1130:
   ;
   ;	};
   ;
	pop	si
	leave
	ret	8
@MOUSE_KEY_STATUS$QNIT1	endp

	public	@MOUSE_KEY_STATUS$QNIT1
	extrn   @SET_POSITION$QII
	extrn   @POSITION_STATUS$QNIT1T1:far
	extrn	@CALLRSCOMMAND$QNI:far
end
