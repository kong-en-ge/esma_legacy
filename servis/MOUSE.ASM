	  .286
	  .model LARGE graphics
.DATA
  __dec	     label  word
  dw   4
  _MOUSE_READY	label	word
  dw   1

_BSS	segment word public 'BSS'
  _X_MIN	label	word
  dw  (?)
  _X_MAX	label	word
  dw  (?)
  _Y_MIN	label	word
  dw  (?)
  _Y_MAX	label	word
  dw  (?)
_BSS	ends

.CODE graphics

;int init_mouse(void) {
@init_mouse$qv	proc	far
	push	si
	push	di
	xor	  ax,ax
	int	  33h
	cmp	ax,65535
	jne	short @1@242
	mov	word ptr DGROUP:_MOUSE_READY,0
@1@242:
	pop	di
	pop	si
	ret
@init_mouse$qv	endp

;void pascal mouse_show(int show) {
@MOUSE_SHOW$QI	proc	far
	push	bp
	mov	bp,sp
	push	si
	push	di
;if (MOUSE_READY) return;
	cmp	word ptr DGROUP:_MOUSE_READY,0
	jne	short @2@218
;mov  ax,show
	mov	  ax,[bp+6]
	int	  33h
@2@218:
	pop	di
	pop	si
	pop	bp
	ret	2
@MOUSE_SHOW$QI	endp

;void pascal mouse_screen_size(int x_min, int y_min, int x_max, int y_max) {
@MOUSE_SCREEN_SIZE$QIIII	proc	far
	push	bp
	mov	bp,sp
	push	si
	push	di
;if (MOUSE_READY) return;
	cmp	word ptr DGROUP:_MOUSE_READY,0
	jne	short @3@674
;mov  cx,x_min
	mov	  cx,[bp+12]
;mov  dx,x_max
	mov	  dx,[bp+8]
	cmp    cx,dx
	jle    @3@680
	xchg   cx,dx
@3@680:
	mov	word ptr DGROUP:_X_MIN,cx
	mov	word ptr DGROUP:_X_MAX,dx
	mov	  ax,7
	int	  33h
;mov  cx,y_min
	mov	  cx,[bp+10]
;mov  dx,y_max
	mov	  dx,[bp+6]
	cmp    cx,dx
	jle    @3@700
	xchg   cx,dx
@3@700:
	mov	word ptr DGROUP:_Y_MIN,cx
	mov	word ptr DGROUP:_Y_MAX,dx
	mov	  ax,8
	int	  33h
@3@674:
	pop	di
	pop	si
	pop	bp
	ret	8
@MOUSE_SCREEN_SIZE$QIIII	endp

;void pascal position_status(int *Status, int *Xcoord, int *Ycoord) {
@POSITION_STATUS$QNIT1T1	proc	far
	push	bp
	mov	bp,sp
	push	si
	push	di
;if (MOUSE_READY) {*Status=0;return;};
	cmp	word ptr DGROUP:_MOUSE_READY,0
	je	short @4@74
	les	bx,dword ptr [bp+14]
	mov	word ptr es:[bx],0
	jmp	short @4@362
@4@74:
	mov	  ax,3
	int	  33h
;les  si,Status
	les	  si,[bp+14]
	mov	  es:[si],bx
;les  si,Ycoord
	les	  si,[bp+6]
	mov	  es:[si],dx
;les  si,Xcoord
	les	  si,[bp+10]
	mov    es:[si],cx
@4@362:
	pop	di
	pop	si
	pop	bp
	ret	12
@POSITION_STATUS$QNIT1T1	endp

;void pascal set_position(int x, int y) {
@SET_POSITION$QII	proc	far
	push	bp
	mov	bp,sp
	push	si
	push	di
;if (MOUSE_READY) return;
	cmp	word ptr DGROUP:_MOUSE_READY,0
	jne	short @5@266
@5@74:
	mov	  ax,4
;mov  cx,x
	mov	  cx,[bp+8]
;mov  dx,y
	mov	  dx,[bp+6]
	int	  33h
@5@266:
	pop	di
	pop	si
	pop	bp
	ret	4
@SET_POSITION$QII	endp

;void pascal mouse_store(void *buff) {
@MOUSE_STORE$QNV	proc	far
	push	bp
	mov	bp,sp
	push	si
	push	di
;if (MOUSE_READY) return;
	cmp	word ptr DGROUP:_MOUSE_READY,0
	jne	short @7@266
;les  si,buff
	les  si,[bp+6]
	mov	  dx,si
	mov	  ax,16h
	int	  33h
@7@266:
	pop	di
	pop	si
	pop	bp
	ret	4
@MOUSE_STORE$QNV	endp

;void pascal mouse_restore(void *buff) {
@MOUSE_RESTORE$QNV	proc	far
	push	bp
	mov	bp,sp
	push	si
	push	di
;if (MOUSE_READY) return;
	cmp	word ptr DGROUP:_MOUSE_READY,0
	jne	short @8@266
;les  si,buff
	les	  si,[bp+6]
	mov	  dx,si
	mov	  ax,17h
	int	  33h
@8@266:
	pop	di
	pop	si
	pop	bp
	ret	4
@MOUSE_RESTORE$QNV	endp

;void mouse_reset(void) {
@mouse_reset$qv	proc	far
	push	bp
	mov	bp,sp
	push	si
	push	di
;if (MOUSE_READY) return;
	cmp	word ptr DGROUP:_MOUSE_READY,0
	jne	short @9@266
	mov	  ax,21h
	int	  33h
;if (_AX==0x21) MOUSE_READY=1;
	cmp	ax,33
	jne	short @9@266
	mov	word ptr DGROUP:_MOUSE_READY,1
@9@266:
	pop	di
	pop	si
	pop	bp
	ret
@mouse_reset$qv	endp

;void pascal mouse_setgraphcursor(int x0,int y0,const void *mousecur) {
@MOUSE_SETGRAPHCURSOR$QIINXV	proc	far
	push	bp
	mov	bp,sp
	push	si
	push	di
;if (MOUSE_READY) return;
	cmp	word ptr DGROUP:_MOUSE_READY,0
	jne	short @11@314
;mov  bx,x0
	mov	  bx,[bp+12]
;mov  cx,y0
	mov	  cx,[bp+10]
;les  si,mousecur
	les	  si,[bp+6]
	mov	  dx,si
	mov	  ax,9h
	int	  33h
@11@314:
	pop	di
	pop	si
	pop	bp
	ret	8
@MOUSE_SETGRAPHCURSOR$QIINXV	endp

;unsigned mouse_size_buff(void) {
@mouse_size_buff$qv	proc	far
	push	bp
	mov	bp,sp
	push	si
	push	di
;if (MOUSE_READY) return 0;
	cmp	word ptr DGROUP:_MOUSE_READY,0
	je	short @12@74
	xor	ax,ax
	jmp	short @12@242
@12@74:
	mov	  ax,15h
	int	  33h
	mov	ax,bx
@12@242:
	pop	di
	pop	si
	pop	bp
	ret
@mouse_size_buff$qv	endp
	public	__dec
	public	_X_MIN
	public	_X_MAX
	public	_Y_MIN
	public	_Y_MAX
	public	_MOUSE_READY
	public	@init_mouse$qv
	public	@MOUSE_SHOW$QI
	public	@MOUSE_SCREEN_SIZE$QIIII
	public	@POSITION_STATUS$QNIT1T1
	public	@SET_POSITION$QII
	public	@MOUSE_STORE$QNV
	public	@MOUSE_RESTORE$QNV
	public	@mouse_reset$qv
	public	@MOUSE_SETGRAPHCURSOR$QIINXV
	public	@mouse_size_buff$qv
	extrn	_bioskey:far
	extrn	@CALLRSCOMMAND$QNI:far
end
