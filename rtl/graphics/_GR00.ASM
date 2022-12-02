	.8086
        PAGE 80,120
        %CONDS
        .model LARGE graphics
        .DATA
viewporttype    STRUC
        _LEFT   dw      ?
        _TOP    dw      ?
        _RIGHT  dw      ?
        _BOTTOM dw      ?
        _CLIP   dw      ?
ENDS

extrn   _VIEW                   :viewporttype
extrn   _CUR_COLOR              :byte
extrn   _CUR_BKCOLOR            :byte
extrn   _CUR_FILLCOLOR          :byte

extrn   _CUR_LINE_STYLE         :byte
extrn   _CUR_LINE_PATTERN       :word
extrn   _CUR_LINE_THICK;        :byte
extrn   _CUR_FILL_STYLE         :byte
extrn   _CUR_FILL_PATTERN       :byte
extrn   _CUR_PALETTE            :byte
extrn   _CUR_PAGE_ADDR          :word
extrn   _CUR_WRITE_MODE         :byte
extrn   _CUR_TJUST_HOR          :byte
extrn   _CUR_TJUST_VER          :byte
extrn   _CUR_X                  :word
extrn   _CUR_Y                  :word
extrn   _CUR_BUFF_PTR           :dword
datatable label byte
        db      0,1,3,0,2,81h,82h,0,4,84h,83h



word_var        dw      0
left_mask       db      0FFh, 7Fh, 3Fh, 1Fh, 0Fh, 07h, 03h, 01h
right_mask      db       80h,0C0h,0E0h,0F0h,0F8h,0FCh,0FEh,0FFh
bit_mask        db       80h, 40h, 20h, 10h, 08h, 04h, 02h, 01h
__line_mask     dw      0
__dx_line       db      0,0
                ;       -1,1,0,0,1,-1,0,0,-2,2,0,0,2,-2
__dy_line       db      -1,1,0,0,1,-1,0,0,-2,2,0,0,2,-2,0,0

__line_number   dw      0
__line_mask_5   dw      0, 0, 0, 0, 0


mode_mask       db      0,18h,10h,8,0

__line_size     equ     50h

        .CODE   GRAPHICS
CALCM	MACRO m2,d1,a1
;	mov	ax,m1
	imul	m2
	idiv	d1
	add	ax,a1
ENDM

cs_word_var     dw      0
cs_byte_var     db      0

PROC PASCAL BAR  far left:word,top:word,right:word,bottom:word
USES    SI,DI
        call    __prebar
        call    _bar_clip
        call    __onebar
@@b00:
	call    __postbar
	ret
b09:
	pop	cx
	jmp	@@b00
BAR     ENDP
_bar_clip proc near
        mov     ax,left
        mov     bx,top
        mov     cx,right
        mov     dx,bottom
        cmp     ax,cx
        jle     b00
        xchg    ax,cx
b00:
        cmp     bx,dx
        jle     b01
        xchg    bx,dx
b01:
	or      cx,cx
	jl	b09
	or      ax,ax
	jg      b02
	xor     ax,ax
b02:
	mov     si,_VIEW._LEFT
	mov     di,_VIEW._RIGHT
	add     ax,si
	cmp     ax,di
	jg      b09
	add     cx,si
	cmp     cx,di
	jle     b03
	mov     cx,di
b03:
	or      dx,dx
	jl	b09
	or      bx,bx
	jg      b04
	xor     bx,bx
b04:
	mov     si,_VIEW._TOP
	mov     di,_VIEW._BOTTOM
	add     bx,si
	cmp     bx,di
	jg      b09
	add     dx,si
	cmp     dx,di
	jle     b05
	mov     dx,di
b05:
	ret
_bar_clip endp
__prebar proc near
        mov     dx,3CEh
        mov     ax,205h                 ; port 3CEh, EGA graphic index
        out     dx,ax                   ;  al = 5, mode
__pre_entry:
	cmp     byte ptr _CUR_WRITE_MODE,0
        je      p_copy_mode
        mov     ax,1803h                ; port 3CEh, EGA graphic index
        out     dx,ax                   ; al = 3, data rotate
p_copy_mode:
        mov     al,8                    ; port 3CEh, EGA graphic index
        out     dx,al                   ;  al = 8, data bit mask
        retn
__prebar endp
__onebar2 proc near
        add     ax,_VIEW._LEFT
        add     cx,_VIEW._LEFT
        add     bx,_VIEW._TOP
        add     dx,_VIEW._TOP
__onebar2 endp
__onebar proc near
        push    bp
        mov     si,ax           ;x1
        mov     di,bx           ;y1
        and     bx,7
	add     bx,offset _CUR_FILL_PATTERN
        sub     dx,di
        inc     dx              ;dy+1
        mov     word_var,dx
        push    bx              ;offset pattern
        mov     bx,si           ;x1
        mov     ax,cx           ;x2
        push    cx              ;x2
        mov     cl,3
        shr     ax,cl
        shr     bx,cl
        pop     cx              ;cx-x2
        sub     ax,bx           ;число байт - 1
        pop     bx              ;bx-offset pattern
                                ;на стеке - ничего
        push    ax              ;число байт - 1
        push    cx              ;x2
        push    bx              ;offset pattern
        call    get_addr        ;es:di - адрес для x1,y1
        cld
	pop     si              ;si-offset pattern
        mov     cl,left_mask[bx]
        pop     bx              ;x2
        and     bx,7
        mov     bl,right_mask[bx]
        mov     bh,cl           ;bh-bl - левая - правая маски
        pop     bp              ;число байт - 1
        mov     al,byte ptr _CUR_FILLCOLOR
        mov     ah,al
bar_loop:
;ah-al  - _CUR_FILLCOLOR - маска
;bh-bl - левая - правая маски
;si-offset pattern
;es:di адрес байта
;bp    - число байт
        lodsb                           ; pattern
        or      al,al
        jz      empty_line
        push    bx
        push    di
	call    fill_line
        pop     di
        pop     bx
empty_line:
        cmp     si,offset _CUR_FILL_PATTERN+8
        jb      loc_73
        mov     si,offset _CUR_FILL_PATTERN
loc_73:
        add     di,__line_size
        dec     word ptr word_var
        jnz     bar_loop
        pop     bp
        ret

;ah-al  - _CUR_FILLCOLOR-маска заливки
;bh-bl - левая - правая маски
;es:di адрес
;BP-число байт
fill_line       proc    near
        mov     cx,bp
	mov     dx,3CFh
        push    bx
        mov     bl,bh
        mov     bh,al
        jcxz    loc_74          ;если конец и начало в одном байте
        call    sub_44
        mov     bl,0FFh
        dec     cx
        jcxz    loc_74
        call    sub_45
loc_74:
        pop     cx
        and     bl,cl

;ah-    _CUR_FILLCOLOR
;bh-bl  маска заливки - маска байта
;
;Увеличивает di
sub_44:
        mov     al,bh     ;вывод одного байта
	and     al,bl
        out     dx,al
        mov     al,ah
        xchg    al,es:[di]
        inc     di
        retn
fill_line       endp

;ah-    _CUR_FILLCOLOR
;bh-bl  маска заливки - маска байта
;
;Увеличивает di

; Рисует среднюю часть линии при заливке
; DI - первый адрес в строке
; CX - число байт в строке
; AH - _CUR_FILLCOLOR
; BL - маска байта - 0xff
; BH - маска заливки
sub_45          proc    near
	mov     al,bh
        out     dx,al                   ; port 3CFh, EGA graphic func
        cmp     al,ah
        jne     loc_75
;       mov     al,ah
        rep     stosb
        retn
loc_75:
        mov     al,bh
        out     dx,al                   ; port 3CFh, EGA graphic func
locloop_77:
        mov     al,ah
        xchg    al,es:[di]
        inc     di
        loop    locloop_77
        retn
sub_45  endp

__ONEBAR endp

__POSTBAR proc near
      mov       dx,3CEh
      mov       ax,5            ; port 3CEh, EGA graphic index
      out       dx,ax           ; al = 5, mode
      mov       ax,3            ; port 3CEh, EGA graphic index
      out       dx,ax           ; al = 3, data rotate
      mov       ax,0FF08h       ; port 3CEh, EGA graphic index
      out       dx,ax           ; al = 8, data bit mask
      mov       ax,0F02h
      mov       dx,3C4h         ; port 3C4h, EGA sequencr index
      out       dx,ax           ; al = 2, map mask register
      retn
__POSTBAR endp

; на входе - DI -y
;            SI -x
; на выходе -ES:DI - адрес байта
;            BX    - номер бита
; портит SI,CL
;
get_addr        proc    near
        shl     di,1    ;di*80
        mov     bx,di
        mov     cl,2
        shl     di,cl
        add     di,bx
        inc     cl
        shl     di,cl
        mov     bx,si
        shr     si,cl   ;si/8
        add     di,si
        and     bx,7
        mov     es,_CUR_PAGE_ADDR
        retn
get_addr        endp

PROC PASCAL LINE far x1:word,y1:word,x2:word,y2:word
USES    SI,DI
        mov     ax,x2
        mov     _CUR_X,ax
	mov     ax,y2
        mov     _CUR_Y,ax
        call    __preline
        call    __addxymin
        call    __line_entry
        call    __postline
        ret
LINE     ENDP

PROC PASCAL LINE2 far x1:word,y1:word,x2:word,y2:word
USES    SI,DI
        call    __addxymin
        call    __line_entry
        ret
LINE2   ENDP

PROC PASCAL LINE3 far x1:word,y1:word,x2:word,y2:word
USES    SI,DI
        call    __line_entry
        ret
LINE3   ENDP

__line_entry proc near
        mov     ax,x1
        mov     bx,y1
        mov     cx,x2
        mov     dx,y2
        mov     __line_number,0
lt_start:
        call    __clip
        jc      l_next
        call    __oneline
l_next:
        mov     di,__line_number
        mov     bx,di
        shl     di,1
        mov     ax,__line_mask
        rol     ax,1
        mov     __line_mask_5[di],ax
        inc     bx
	cmp     bx,_CUR_LINE_THICK
        jge     lend
        mov     __line_number,bx

        xor     bx,bx
        mov     cx,x1
        sub     cx,x2
        jge     l00
        inc     bx
        neg     cx
l00:
        mov     dx,y1
        sub     dx,y2
        jge     l02
        neg     dx
        cmp     dx,cx
        jle     l03
        mov     bl,2
        jmp     l03
l02:
	cmp     dx,cx
        jle     l03
        mov     bl,3
l03:
        inc     di
        inc     di
        mov     ax,__line_mask_5[di]
        mov     __line_mask,ax
        shl     di,1
        mov     al,__dy_line[bx+di-4]
        cbw
        mov     dx,ax
        mov     al,__dx_line[bx+di-4]
        cbw
        mov     bx,dx
        mov     cx,ax
        add     ax,x1
        add     bx,y1
        add     cx,x2
        add     dx,y2
	jmp     short   lt_start
lend:
        mov     ax,__line_mask_5
        mov     __line_mask,ax
        ret
__line_entry endp
__addxymin proc near
        mov     ax,_VIEW._LEFT
        add     x1,ax
        add     x2,ax
        mov     ax,_VIEW._TOP
        add     y1,ax
        add     y2,ax
        retn
__addxymin endp

__preline  proc near
        mov     si,offset _CUR_LINE_PATTERN
        mov     di,offset __line_mask_5
        push    ds
        pop     es
        cld
        mov     cx,5
        rep     movsw
        mov     ax,_CUR_LINE_PATTERN
        mov     __line_mask,ax
        mov     dx,3CEh
        xor     al,al
        mov     ah,_CUR_COLOR           ; port 3CEh, EGA graphic index
        out     dx,ax                   ; al = 0, set/reset bit
        mov     ax,0ff01h               ; port 3CEh, EGA graphic index
        out     dx,ax                   ; al = 1, enable set/reset
        jmp     __pre_entry
        retn
__preline  endp

;AX-BX-CX-DX - координаты x1,y1,x2,y2
;

PROC __oneline  near
        push    bp
        mov     si,ax
        mov     di,bx

        sub     dx,bx
        mov     bl,__line_size

        push    ds
        push    cs
        pop     ds
assume  ds:     @code

        jge     ol00
        neg     bl
        neg     dx
ol00:
        mov     label_c2[2],bl
        mov     label_c3[2],bl
        sub     ax,cx
        mov     bx,0C04Fh
        mov     cx,0C4DFh
        jge     ol01
        sub     bx,0F808h
        sub     cx,0F808h
        neg     ax
ol01:
        mov     label_c7,bl             ;inc di
        mov     label_c0[1],bh          ;ror al,1
        mov     label_c1[1],cl          ;adc di,0
        mov     label_c5[1],cl          ;adc di,0
        mov     label_c4[1],ch          ;ror ah,1
        mov     label_c6[1],ch          ;ror ah,1
        pop     ds
assume  ds:     @data
        call    get_addr
        mov     si,dx
        mov     bp,ax
        mov     al,bit_mask[bx]
        mov     dx,3CFh
        cmp     si,bp
        jb      ol_yx
ol_xy:
        mov     cx,si
        inc     cx
        shl     bp,1
        mov     bx,bp
        sub     bp,si
        shl     si,1
        sub     bx,si
        add     si,bx
        out     dx,al

ol_xy_loop:
        ror     word ptr __line_mask,1
        jnc     ol10
        xchg    ah,es:[di]
ol10:
        and     bp,bp
        jns     ol11
        add     bp,si
        jmp     short ol12
ol11:
label_c0 label  byte
        ror     al,1
label_c1 label  byte
        adc     di,0
        out     dx,al                   ; port 3CFh, EGA graphic func
        add     bp,bx
ol12:
label_c2 label  byte
        add     di,__line_size
        loop    ol_xy_loop
        jmp     short ol_exit

ol_yx:
        mov     cx,bp
        inc     cx
        shl     si,1
        mov     bx,si
        sub     bx,bp
        shl     bp,1
        sub     si,bp
        add     bp,si
        mov     ah,al
        sub     al,al

ol_yx_loop:
        ror     word ptr __line_mask,1
        jnc     ol_20
        or      al,ah
ol_20:
        and     bx,bx
        jns     ol_21
        add     bx,bp
        jmp     short ol_22
ol_21:
        out     dx,al                   ; port 3CFh, EGA graphic func
        xchg    al,es:[di]
        sub     al,al
label_c3 label  byte
        add     di,__line_size
        add     bx,si
label_c4 label  byte
        ror     ah,1
label_c5 label  byte
        adc     di,0
        jmp     ol_23
ol_22:
label_c6 label  byte
        ror     ah,1
        jnc     ol_23
        out     dx,al                   ; port 3CFh, EGA graphic func
        xchg    al,es:[di]
label_c7 label  byte
        inc     di
        sub     al,al
ol_23:
        loop    ol_yx_loop

        out     dx,al                   ; port 3CFh, EGA graphic func
        xchg    al,es:[di]
ol_exit:
        pop     bp
        retn
__oneline  endp
__postline proc near
        call    __postbar
        mov     dx,03ceh
        mov     ax,1                    ; port 3CFh, EGA graphic index
        out     dx,ax                   ; al = 1, enable set/reset
        retn
__postline endp
PROC PASCAL GETIMAGE far left:word,top:word,right:word,bot:word,bitmap:dword
USES    SI,DI,DS
        add     bp,4
        call    _bar_clip
        sub     bp,4
        cld
        mov     si,ax
        sub     cx,ax
        sub     dx,bx
        les     di,bitmap
        mov     ax,cx
        stosw
        mov     ax,dx
        stosw
        inc     dx
        mov     cs:cs_word_var,dx
        push    es
        push    di
        push    cx
        mov     di,bx
        call    get_addr
        mov     cl,bl
        pop     bx
        mov     dx,bx
        and     bx,7
        mov     bl,right_mask[bx]
        push    es
        pop     ds
        mov     si,di
        pop     di
        pop     es
        shr     dx,1
        shr     dx,1
        shr     dx,1
        mov     bp,dx
gm_line_loop:
        mov     ch,3
gm_map_loop:
        push    cx
        push    si
        mov     dx,3CEh
        mov     al,4
        mov     ah,ch                   ; port 3CEh, EGA graphic index
        out     dx,ax                   ; al = 4, read map select
        mov     dx,bp
        lodsb
        mov     ah,al
gm_byte_loop:
        lodsb
        mov     ch,al
        shl     ax,cl
        mov     al,ah
        stosb
        mov     ah,ch
        dec     dx
        jns     gm_byte_loop
        and     es:[di-1],bl
        pop     si
        pop     cx
        dec     ch
        jns     gm_map_loop
        add     si,__line_size
        dec     cs:cs_word_var
        jnz     gm_line_loop
        call    __postbar
        ret
GETIMAGE        ENDP

PROC PASCAL PUTIMAGE far left:word,top:word,bitmap:dword,op:word
USES    SI,DI,DS
        mov     bx,op
        mov     ah,mode_mask[bx]
        mov     al,3
        mov     dx,3CEh
        out     dx,ax
        mov     al,8
        out     dx,al
        mov     ax,09090h
        cmp     bl,4
        jnz     pi00
        mov     ax,0D0F6h
pi00:
        mov     cs:label_c10,ax
        mov     cs:label_c11,ax
        mov     cs:label_c12,ax
        cld
        push    ds
        lds     si,bitmap
        lodsw
        mov     cx,ax
        mov     bx,ax
        lodsw
        pop     ds
        push    si
        inc     ax
        mov     cs:cs_word_var,ax
        mov     al,cl
        and     al,7
        sub     al,7
        neg     al
        mov     cs:cs_byte_var,al
        shr     cx,1
        shr     cx,1
        shr     cx,1
;       inc     cx
        mov     si,left
        add     si,_VIEW._LEFT
        add     bx,si           ;x2
        mov     di,top
        add     di,_VIEW._TOP   ;y1
        push    cx
        push    bx
        call    get_addr
        mov     ch,left_mask[bx]
        mov     cl,bl
        pop     bx
        and     bx,7
        mov     bl,right_mask[bx]
        mov     bh,ch
        mov     ds,word ptr bitmap[2]
        pop     bp
        pop     si
pi_line_loop:
        mov     ch,4
pi_map_loop:
        push    bx
        push    cx
        push    bp
        push    di
        call    pi_proc
        pop     di
        pop     bp
        pop     cx
        pop     bx
        dec     ch
        jnz     pi_map_loop
        add     di,__line_size
        dec     cs:cs_word_var
        jnz     pi_line_loop
        call    __postbar
        ret
PUTIMAGE ENDP
pi_proc proc near
        mov     dx,3C4h
        mov     ax,8002h
        xchg    ch,cl
        rol     ah,cl
        xchg    ch,cl
        out     dx,ax
        mov     dl,0CFh
        push    bx
        lodsb
;       dec     bp
        xor     ah,ah
        mov     ch,al
        mov     bl,bh
pip00:
        shr     ax,cl
        and     bp,bp
        jz      pip01
        mov     ah,al
        mov     al,bl
        out     dx,al
        mov     al,ah
label_c10 label word
        not     al
        mov     ah,es:[di]
        stosb
        mov     bl,0FFh
        mov     ah,ch
        lodsb
        mov     ch,al
        dec     bp
        jmp     short pip00
pip01:
        mov     dh,cs:cs_byte_var
        cmp     dh,cl
        pop     dx
        jge     pip02
        push    dx
        mov     dx,3CFh
        mov     ah,al
        mov     al,bl
        out     dx,al
        mov     al,ah
label_c11 label word
        not     al
        mov     ah,es:[di]
        stosb
        pop     dx
        mov     ah,ch
        xor     al,al
        shr     ax,cl
        mov     bl,0FFh
pip02:
        and     bl,dl
        mov     dx,3CFh
        mov     ah,al
        mov     al,bl
        out     dx,al
        mov     al,ah
label_c12 label word
        not     al
        xchg    al,es:[di]
;       mov     ah,es:[di]
;       mov     es:[di],al
        ret
pi_proc endp

PROC PASCAL GETPIXEL far x:word,y:word
USES    SI,DI
        mov     si,x
        mov     di,y
        add     si,_VIEW._LEFT
        add     di,_VIEW._TOP
        call    get_addr
        mov     cl,7
        sub     cl,bl
        mov     dx,3CEh
        mov     ax,304h
        mov     ch,1
        shl     ch,cl
        xor     bl,bl
gp_map_loop:
        out     dx,ax                   ; port 3CEh, EGA graphic index
                                        ;  al = 4, read map select
        mov     bh,es:[di]
        and     bh,ch
        shr     bh,cl
        shl     bl,1
        or      bl,bh
        dec     ah
        jge     gp_map_loop
        mov     al,bl
        inc     ah
        ret
GETPIXEL endp
PROC PASCAL PUTPIXEL far x:word,y:word,color:word
USES    SI,DI
        call    __prebar
        mov     si,x
        mov     di,y
        add     si,_VIEW._LEFT
        add     di,_VIEW._TOP
        call    get_addr
        mov     dx,3CFh
        mov     al,bit_mask[BX]
        out     dx,al                   ; port 3CFh, EGA graphic func
        mov     ax,color
        xchg    al,es:[di]
        call    __postbar
        ret
PUTPIXEL endp

PROC PASCAL PUTMARKER_8_N  far x:word,y:word,n:word,mar:dword
USES    SI,DI
        call    __prebar
        mov     si,x
        mov     di,y
        add     si,_VIEW._LEFT
        add     di,_VIEW._TOP
        les     bx,mar
        mov     cx,n
        call    out_8_n_symbol
        call    __postbar
        ret
PUTMARKER_8_N endp
PROC PASCAL PUTMARKER_16_N  far x:word,y:word,n:word,mar:dword
USES    SI,DI
        call    __prebar
        mov     si,x
        mov     di,y
        add     si,_VIEW._LEFT
        add     di,_VIEW._TOP
        les     bx,mar
        mov     cx,n
        push    si
        push    di
        call    out_8_n_symbol
        pop     di
        pop     si
        add     si,8
        les     bx,mar
        mov     cx,n
        add     bx,16
        call    out_8_n_symbol
        call    __postbar
        ret
PUTMARKER_16_N endp

;es:bx - таблица 8x8
;si,di - x,y
;cx    - n (высота маркера)
out_8_n_symbol proc near
        mov     ax,di
        add     ax,cx
        mov     dx,_VIEW._TOP
        cmp     di,dx
        jge     ot_00
        sub     di,dx
        add     cx,di
        jle     ot_en2
        sub     bx,di
        mov     di,dx
ot_00:
        dec     ax
        mov     dx,_VIEW._BOTTOM
        cmp     ax,dx
        jle     ot_01
        sub     ax,dx
        sub     cx,ax
        jle     ot_en2
        mov     ax,dx
ot_01:
        cmp     di,ax
        jle     ot_05
ot_en2:
        jmp     out_end
ot_05:
        mov     cs:cs_word_var,cx
        mov     ax,si
        add     ax,7
        mov     cx,700h

        mov     dx,_VIEW._LEFT
        cmp     si,dx
        jge     ot_02
        sub     si,dx
        sub     cx,si
        mov     si,dx
ot_02:
        mov     dx,_VIEW._RIGHT
        cmp     ax,dx
        jle     ot_03
        sub     ax,dx
        sub     ch,al
        mov     ax,dx
ot_03:
        cmp     si,ax
        jg      out_end

        push    ds
        push    bp
        push    es
        push    bx
        xor     bx,bx
        mov     bl,ch
        mov     ch,right_mask[bx]
        mov     bl,cl
        and     ch,left_mask[bx]
        mov     cs:os_label[1],ch
        push    ax
        push    cx
        call    get_addr
        pop     cx
        pop     dx
        sub     cl,bl
        neg     cl
        and     cl,7
        xor     bp,bp
        mov     ah,left_mask[bx]
        and     dl,7
        cmp     dl,bl
        adc     bp,0
        mov     bl,dl
        mov     al,right_mask[bx]
        mov     bx,ax
        cld
        pop     si
        mov     ch,_CUR_COLOR
        pop     ds
os_loop:
        lodsb
os_label label byte
        and     al,0
        push    bx
        push    di
        push    cx
        mov     ah,ch
        ror     al,cl
        call    fill_line
        pop     cx
        pop     di
        pop     bx
        add     di,__line_size
        dec     cs:cs_word_var
        jnz     os_loop
        pop     bp
        pop     ds
out_end:
        ret
out_8_n_symbol endp
PROC PASCAL OUTTEXTXY far x:word,y:word,text:dword
USES    SI,DI
        call    __prebar
        les     di,text
        cld
        mov     cx,0FFFFh
        xor     ax,ax
        repnz   scasb
        not     cx
        dec     cx
        shl     cx,1
        shl     cx,1
        mov     si,x
        add     si,_VIEW._LEFT
        cmp     byte ptr _CUR_TJUST_HOR,1
        jl      ot01
        je      ot00
        shl     cx,1
ot00:
        sub     si,cx
ot01:
        mov     di,y
        add     di,_VIEW._TOP
        mov     cx,4
        cmp     byte ptr _CUR_TJUST_VER,1
        jg      ot03
        je      ot02
        shl     cx,1
ot02:
        sub     di,cx
ot03:
        les     bx,text
        mov     al,es:[bx]
        or      al,al
        jz      ot_exit
        xor     ah,ah
        lea     bx,FONT_8_8
        push    cs
        pop     es
        shl     ax,1
        shl     ax,1
        shl     ax,1
        add     bx,ax
        push    si
        push    di
        mov     cx,8
        call    out_8_n_symbol
        pop     di
        pop     si
        add     si,8
        inc     word ptr text
        jmp     ot03
ot_exit:
        mov     _CUR_X,si
	mov     _CUR_Y,di
	call    __postbar
	ret
OUTTEXTXY endp
PROC PASCAL DRAWPOLY  num:word, points:dword
	mov     cx,num
	cmp     cx,1
	jle      @@dp00
	call    fp_copy_buf pascal,num
	call	fp_exit1
@@dp00:
	ret
DRAWPOLY ENDP
PROC PASCAL FILLPOLY  num:word, points:dword
	mov     cx,num
	cmp     cx,2
	jle     @@fp00
	cmp	cx,1000d
	jge	@@fp00
	call    fp_copy_buf
	call	fillentry pascal,num
@@fp00:
	ret
FILLPOLY  endp
fillentry proc pascal near num:word
USES    SI,DI
LOCAL   gortbl:word,curtbl:word,cury:word,maxy:word
	les	di,_CUR_BUFF_PTR
	mov	bx,num
	mov	cx,bx
	shl	bx,1
	shl	bx,1
	shl	bx,1
	mov	ax,es:[di]
	mov	es:[di+bx],ax
	inc     di
	inc     di
	mov	ax,es:[di]
	mov	es:[di+bx],ax
	add	bx,di
	add	bx,4
	mov	gortbl,bx

;	dec     cx
	mov     ax,es:[di]
	mov     bx,ax
	mov     dx,ax
fp_loop1:
	add     di,8
	mov     ax,es:[di]
	cmp     bx,ax
	jle     fp_01
	mov     bx,ax
fp_01:
	cmp     dx,ax
	jge     fp_02
	mov     dx,ax
fp_02:
	loop    fp_loop1

;        cmp     bx,_VIEW._TOP           ;ymin
;        jge     fp_03
;        mov     bx,_VIEW._TOP
;fp_03:
	mov     cury,bx

	cmp     dx,_VIEW._BOTTOM
	jle     fp_04
	mov     dx,_VIEW._BOTTOM
fp_04:
	mov     maxy,dx
	cmp     bx,dx
	jle     fp_05
	jmp     @@fp_exit
fp_05:
	call    __prebar
fp_06:
	mov     curtbl,0
	mov     di,word ptr _CUR_BUFF_PTR
	mov     cx,num
fp_000:
	mov     ax,es:[di]
	mov     bx,es:[di+2]
	mov     si,es:[di+8]
	mov     dx,es:[di+10]
	cmp     bx,dx
	jle     fp_001
	xchg    ax,si
	xchg    bx,dx
fp_001:
	cmp     bx,cury
	jg      fp_007
	je      fp_002
	cmp     dx,cury
	jle     fp_007
; Отрезок обрабатывается более одного раза
	sub     dx,bx
	mov     bx,1
	sub     si,ax
	jge     fp_003
	neg     si
	neg     bx
fp_003:
	shl     si,1
	mov	ax,dx
	shl     dx,1
;e[edge]+=(2*(x1-x0));
	add     es:[di+6],si
; while(e[edge] > (int)(y1-y0))
; {
;     e[edge]-=(2*(y1-y0));
;     ++x[edge];
; }
fp_005:
	cmp     ax,es:[di+6]
	jge	fp_006
	sub	es:[di+6],dx
	add     es:[di+4],bx
	jmp     fp_005
fp_002:
; Отрезок впервые обрабатывается ( верхняя граница = cury)
	cmp	bx,dx
	je 	fp_007
	mov     es:[di+4],ax    ; x
	mov     word ptr es:[di+6],0
fp_006:
	mov     ax,es:[di+4]
	call    fp_stx
fp_007:
	add     di,8
	loop    fp_000

fp_end_yloop:
	mov     bx,cury
	cmp     bx,_VIEW._TOP
	jl      fp_e03
	mov     cx,curtbl
	shr     cx,1
	jcxz    fp_e03
	mov     di,gortbl
fp_loop2:
	push    cx
	mov     bx,cury
	mov     ax,es:[di]
	mov     cx,es:[di+2]
	mov     dx,bx
	add     di,4
	cmp     cx,_VIEW._LEFT
	jle     fp_e02
	cmp     ax,_VIEW._RIGHT
	jge     fp_e02
	cmp     cx,_VIEW._RIGHT
	jle     fp_e00
	mov     cx,_VIEW._RIGHT
fp_e00:
	cmp     ax,_VIEW._LEFT
	jge     fp_e01
	mov     ax,_VIEW._LEFT
fp_e01:
	push    es
	push    di
	call    __onebar
	pop     di
	pop     es
fp_e02:
	pop     cx
	loop    fp_loop2
fp_e03:
	mov     ax,cury
	inc     ax
	cmp     ax,maxy
	jg      fp_exit0
	mov     cury,ax
	jmp     fp_06
fp_copy_buf proc near
	push    ds
	mov     bx,_VIEW._LEFT
	mov     dx,_VIEW._TOP
	les     di,_CUR_BUFF_PTR
	lds     si,points
	cld
fp_loop0:
	lodsw
	add     ax,bx
	stosw
	lodsw
	add     ax,dx
	stosw
	add     di,4
	loop    fp_loop0
	pop     ds
@@fp_exit:
	ret
fp_copy_buf endp
fp_stx  proc    near
	push    cx
	mov     si,gortbl
	mov     cx,curtbl
	jcxz    fp_st1
fp_stl:
	cmp     ax,es:[si]
	jl      fp_st0
	inc     si
	inc     si
	loop    fp_stl
	jmp     short fp_st1
fp_st0:
	xchg    ax,es:[si]
	inc     si
	inc     si
	loop    fp_st0
fp_st1:
	mov     es:[si],ax
fp_st2:
	inc     curtbl
	pop     cx
	ret
fp_stx  endp
fp_exit0:
	call    __postbar
fp_exit1:
	call    __preline
	mov     cx,num
	les     si,_CUR_BUFF_PTR
fp_loop_l:
	push    si
	push    es
	push    cx
	call    line3 pascal,es:[si],es:[si+2],es:[si+8],es:[si+10]
	pop     cx
	pop     es
	pop     si
	add     si,8
	loop    fp_loop_l
	call    __postbar
	ret
fillentry endp

proc pascal __clip near
;ax,bx,cx,dx
LOCAL   __x1,__y1,__x2,__y2:word
        mov     __x1,ax
        mov     __y1,bx
        mov     si,ax
        mov     di,bx
        xor     ax,ax
        cmp     si,_VIEW._LEFT
        jge     knxxx
	mov     al,1
knxxx:
        cmp     si,_VIEW._RIGHT
        jle     k1xxx
	mov     al,2
k1xxx:
        cmp     cx,_VIEW._LEFT
        jge     knxnx
        mov     ah,1
knxnx:
        cmp     cx,_VIEW._RIGHT
        jle     kn__x1x
        mov     ah,2
kn__x1x:
        cmp     bx,_VIEW._TOP
        jge     knnnx
        add     al,4
knnnx:
        cmp     bx,_VIEW._BOTTOM
        jle     kn1nx
	add     al,8
kn1nx:
        cmp     dx,_VIEW._TOP
        jge     knnnn
	add     ah,4
knnnn:
        cmp     dx,_VIEW._BOTTOM
        jle     knnn1
        add     ah,8
knnn1:
        mov     bl,al
        or      bl,ah
        je      l2nc

        mov     bh,ah
        and     bh,al
        jnz     exit

        mov     __x2,cx
        mov     __y2,dx

        mov     bl,al
        mov     al,datatable[bx]
        mov     bl,ah
	mov     bh,datatable[bx]
        mov     bl,al
l1:
        or      bl,bl
        js      l1u             ;на расчет угла
        jnz     l1s             ;на расчет стороны
        mov     ax,si
        mov     dx,di
l1nc:
        mov     si,ax
        mov     di,dx
        jmp     short l2
l1s:
        call    calcs           ;pacчет координат пересечения
        jnc     l1nc
        jmp     short exit      ;выход если вне прямоугольника
l1u:
        call    calcu           ;pacчет координат пересечения
        jnc     l1nc
        jmp     short exit      ;выход если вне прямоугольника
l2:
        xchg    bl,bh
        or      bl,bl
        js      l2u             ;на расчет угла
        jnz     l2s             ;на расчет стороны
        mov     cx,__x2
        mov     dx,__y2
l2nc:
        mov     ax,si
        mov     bx,di
        clc
        jmp     short exit1d
l2s:
        call    calcs           ;pacчет координат пересечения
        jmp     short l2us
l2u:
	call    calcu           ;pacчет координат пересечения
l2us:
        mov     cx,ax
        jnc     l2nc
exit:
        stc
exit1d:
        ret

calcs  proc   near
        push    bx
        push    cx
        shl     bx,1
        and     bx,1fh
        mov     dx,bx
        mov     ax,[bx+offset _VIEW._LEFT -2]
        push    ax
        mov     cx,__x1
        sub     cx,__x2
        mov     bx,__y1
	sub     bx,__y2
        test    dl,2
        jz      calcx
calcy:
	sub     ax,__x1
;        mov     dx,__y1
;        call    calc
	CALCM	bx,cx,__y1
        cmp     ax,_VIEW._TOP
        jl      exit0
        cmp     ax,_VIEW._BOTTOM
        jg      exit0
        mov     dx,ax
        pop     ax
exitwell:
        clc
        jmp     exitcalc
exit0:
        pop     cx
        stc
exitcalc:
	pop     cx
        pop     bx
        ret
calcx:
	sub     ax,__y1
;        xchg    bx,cx
;        mov     dx,__x1
;        call    calc
	CALCM	cx,bx,__x1
        cmp     ax,_VIEW._LEFT
        jl      exit0
        cmp     ax,_VIEW._RIGHT
        jg      exit0
        pop     dx
        jmp     exitwell
calcs  endp
calcu  proc near
        push    bx
        and     bl,07h
        call    calcs
        jnc     excu
	pop     bx
        push    bx
        and     bl,07h
        inc     bl
	cmp     bl,4
        jle     cu01
        mov     bl,1
cu01:
        call    calcs
excu:
        pop     bx
        ret
calcu  endp
__clip endp
FONT_8_8 label byte
%NOLIST
include font8_8.txp
%LIST

public  BAR
public  LINE
public  LINE2
public  PUTIMAGE
public  GETIMAGE
public  PUTPIXEL
public  GETPIXEL
public  OUTTEXTXY
public  FILLPOLY
public  DRAWPOLY
public  PUTMARKER_16_N
public  PUTMARKER_8_N
public  fillentry

public  __preline
public  __line_entry
public  __postline
public  __prebar
public  __onebar2
public  __postbar

public  __line_mask_5
public  __line_mask
END

