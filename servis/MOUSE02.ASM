	 .286
	 .model large graphics
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
;void pascal mouse_speed(int horizontal, int vertical){
@MOUSE_SPEED$QII	proc	far
	push	bp
	mov	bp,sp
	push	si
	push	di
;if (MOUSE_READY) return;
	cmp	word ptr DGROUP:_MOUSE_READY,0
	jne	short @6@266
	mov	  ax,15
;mov  cx,horizontal;
	mov	  cx,[bp+8]
;mov  dx,vertical;
	mov	  dx,[bp+6]
	int	  33h
@6@266:
	pop	di
	pop	si
	pop	bp
	ret	4
@MOUSE_SPEED$QII	endp

	public	@MOUSE_SPEED$QII
end