           .286
           PAGE 80,120
           %CONDS
         .MODEL LARGE graphics
        .CODE graphics

CALCM   MACRO m2,d1,a1
;       mov     ax,m1
        imul    m2
        idiv    d1
        add     ax,a1
ENDM

extrn LINE: far
extrn LINE2: far
extrn __preline:        near
extrn __line_entry:     near
extrn __postline:       near
extrn __prebar:         near
extrn __postbar:        near
extrn __onebar2:        near
extrn fillentry:        near
arsiz equ 1200      ;размер выходного буфера в словах

        .DATA
xmindr  dw 0   ;габариты окна карты
ymaxdr  dw 0
xmaxdr  dw 0
ymindr  dw 0
dxmdr   dw 0
dymdr   dw 0
xmins   dw 0   ;габариты на один пиксель шире
ymaxs   dw 0   ;экрана
xmaxs   dw 0
ymins   dw 0


wxmindr dw 0   ;габариты окна экрана
wymindr dw 0
wxmaxdr dw 0
wymaxdr dw 0

dxwdr   dw 0
dywdr   dw 0

dx2dr   dw 0   ;число дискрет на пиксель
dy2dr   dw 0   ;
dxydr   dw 0   ;

nt      dw 0   ;возврат из drawline
x1d     dw 0
y1d     dw 0
x2d     dw 0
y2d     dw 0

viewporttype    STRUC
        _LEFT   dw      ?
        _TOP    dw      ?
        _RIGHT  dw      ?
        _BOTTOM dw      ?
        _CLIP   dw      ?
ENDS

extrn   _VIEW                   :viewporttype


extrn   _CUR_BUFF_PTR           :dword
extrn   _CUR_LINE_PATTERN       :word
extrn   __line_mask_5           :word
extrn   __line_mask             :word

datatable label byte
        db      0,1,3,0,4,81h,84h,0,2,82h,83h
arean   db 0,0,4,0,6,7,5,0,2,1,3
ugoln   db 0000b,0000b,1001b,1001b,0000b,0101b,0101b,0000b ;0
        db 0000b,0000b,0000b,0000b,1010b,1111b,0101b,0000b ;1
        db 1001b,0000b,0000b,0000b,1010b,1010b,0000b,1001b ;2
        db 1001b,0000b,0000b,0000b,0000b,0000b,0110b,1111b ;3
        db 0000b,1010b,1010b,0000b,0000b,0000b,0110b,0110b ;4
        db 0101b,1111b,1010b,0000b,0000b,0000b,0000b,0000b ;5
        db 0101b,0101b,0000b,0110b,0110b,0000b,0000b,0000b ;6
        db 0000b,0000b,1001b,1111b,0110b,0000b,0000b,0000b ;7
lastpr  db 0,0
orpr    equ lastpr[1]


        .CODE   graphics
setviewmap   proc far
                push bp
                mov  bp,sp
                mov  ax,[bp+6]
                mov  bx,[bp+10]
                mov  ymaxdr,ax
                mov  ymindr,bx
                mov  ymaxs,ax
                mov  ymins,bx
                sub  ax,bx
                mov  dymdr,ax
                mov  ax,[bp+8]
                mov  bx,[bp+12]
                mov  xmaxdr,ax
                mov  xmindr,bx
                sub  ax,bx
                mov  dxmdr,ax
                pop  bp
                ret  8
setviewmap   endp
setview      proc far
                push bp
                mov  bp,sp
                mov  ax,[bp+6]
                mov  bx,[bp+10]
                mov  wymaxdr,ax
                mov  wymindr,bx
                sub  ax,bx
                mov  dywdr,ax
                mov  ax,dymdr
                xor  dx,dx
                div  dywdr
                test ax,ax
                jnz  sv00
                inc  ax
sv00:
                mov  dy2dr,ax
                add  ymaxs,ax
;               add  ymaxs,ax
                jns  sv02
                mov  ymaxs,7fffh
sv02:
                sub  ymins,ax
;               sub  ymins,ax
                add  xmaxs,ax
                jns  sv03
                mov  xmaxs,7fffh
sv03:
                sub  xmins,ax

                mov  ax,[bp+8]
                mov  bx,[bp+12]
                mov  wxmaxdr,ax
                mov  wxmindr,bx
                sub  ax,bx
                mov  dxwdr,ax
                mov  ax,dxmdr
                xor  dx,dx
                div  dxwdr
                test ax,ax
                jnz  sv01
                inc  ax
sv01:
                mov  dx2dr,ax
                pop  bp
                ret  8
setview      endp

;******************************************************
drawline  proc pascal far x1 : word,y1 : word,x2 : word,y2 : word
USES      SI,DI
          call  DLINE  Pascal,x1,y1,x2,y2
          mov   al,byte ptr nt
          cmp   byte  ptr nt,0
          je    @@exit
          call  LINE Pascal,x1d,y1d,x2d,y2d
@@exit:
          ret
drawline  endp
;******************************************************
dline     proc   pascal near x1:word,y1:word,x2:word,y2:word
;       выход   nt - число пересечений (обе точки внутри - 3)
;               x1d-y2d - координаты отрезкав экранных (кроме nt=0)
;       если nt=0 AX,DX - координаты последней рассчитанной точки
;
LOCAL   x1p,y1p,x2p,y2p
        xor     bx,bx
        mov     byte ptr nt,bl

        mov     ax,x1
        cmp     ax,xmindr
        jge     knxxx
        mov     bl,1
knxxx:
        cmp     ax,xmaxdr
        jle     k1xxx
        mov     bl,2
k1xxx:
        mov     ax,x2
        cmp     ax,xmindr
        jge     knxnx
        mov     bh,1
knxnx:
        cmp     ax,xmaxdr
        jle     knx1x
        mov     bh,2
knx1x:
        mov     ax,y1
        cmp     ax,ymindr
        jge     knnnx
        add     bl,4
knnnx:
        cmp     ax,ymaxdr
        jle     kn1nx
        add     bl,8
kn1nx:
        mov     ax,y2
        cmp     ax,ymindr
        jge     knnnn
        add     bh,4
knnnn:
        cmp     ax,ymaxdr
        jle     knnn1
        add     bh,8
knnn1:
        mov     ax,bx
        mov     cl,bl
        or      cl,bh
        jne     l0
        mov     byte ptr nt,3
        xor     bx,bx
        jmp     short l1
l0:
        mov     bh,al
        and     bh,ah
        jz      l00
        jmp     exit
l00:
        mov     bl,al
        mov     al,datatable[bx]
        mov     bl,ah
        mov     bh,datatable[bx]
        mov     bl,al
l1:
        or      bl,bl
        js      l1u             ;на расчет угла
        jnz     l1s             ;на расчет стороны
        dec     byte ptr nt
        mov     ax,x1
        call    perevodx
        mov     dx,y1
        call    perevody
l1nc:
        mov     si,ax
        mov     di,dx
        inc     byte ptr nt
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
        dec     byte ptr nt
        mov     ax,x2
        call    perevodx
        mov     dx,y2
        call    perevody
l2nc:
        inc     byte ptr nt
        jmp     short calline
l2s:
        call    calcs           ;pacчет координат пересечения
        jnc     l2nc
        jmp     short exit      ;выход если вне прямоугольника
l2u:
        call    calcu           ;pacчет координат пересечения
        jnc     l2nc
exit:
        mov     byte ptr nt,0
exit1d:
        ret
calline:
        mov     x1d,si
        mov     y1d,di
        mov     x2d,ax
        mov     y2d,dx
        jmp     short exit1d


;calc   proc    near
;ax-mul1;bx-mul2;cx-div1;dx-add1;
;
;       push    si
;       xor     si,si
;       push    dx
;       imul    bx
;       or      dx,dx
;       jge     c00
;       neg     dx
;       neg     ax
;       sbb     dx,si
;       not     si
;c00:
;       or      cx,cx
;       jge     c01
;       neg     cx
;       not     si
;c01:
;       cmp     dx,cx
;       jge     c04
;       div     cx
;       or      ax,ax
;       js      c04
;       shl     dx,1
;       cmp     dx,cx
;       jb      c02
;       inc     ax
;       js      c04
;c02:
;       xor     ax,si
;       sub     ax,si
;       pop     cx
;       add     ax,cx
;c05:
;       pop     si
;       ret
;c04:
;       pop     cx
;       mov     ax,7fffh
;       xor     ax,si
;       jmp     c05
;calc   endp
calcs  proc   near
        push    bx
        push    cx
        shl     bx,1
        and     bx,1fh
        mov     dx,bx
        mov     ax,[bx+offset xmindr -2]
        push    dx
        mov     cx,x1
        sub     cx,x2
        mov     bx,y1
        sub     bx,y2
        test    dl,2
        jz      calcx
calcy:
        sub     ax,x1
;       mov     dx,y1
;       call    calc
        CALCM   bx,cx,y1
        pop     bx
        mov     dx,ax
        cmp     ax,ymindr
        jl      exit0
        cmp     ax,ymaxdr
        jg      exit0
        call    perevody
        mov     ax,[bx+offset wxmindr -2]
exitwell:
        clc
        jmp     exitcalc
exit0:
        mov     ax,[bx+offset xmindr -2]
@@exit2:
        stc
exitcalc:
        pop     cx
        pop     bx
        ret
calcx:
        sub     ax,y1
;       xchg    bx,cx
;       mov     dx,x1
;       call    calc
        CALCM   cx,bx,x1
        pop     bx
        cmp     ax,xmindr
        jl      @@exit1
        cmp     ax,xmaxdr
        jg      @@exit1
        call    perevodx
        mov     dx,[bx+offset wxmindr -2]
        jmp     exitwell
@@exit1:
        mov     dx,[bx+offset xmindr -2]
        jmp     @@exit2
calcs  endp
calcu  proc near
        push    bx
        and     bl,07h
        call    calcs
        jnc     excu
        pop     bx
        mov     x1p,ax
        mov     y1p,dx
        push    bx
        and     bl,07h
        dec     bl
        jnz     cu01
        mov     bl,4
cu01:
        call    calcs
        jc      @@cu02
excu:
        pop     bx
        ret
@@cu02:
        mov     x2p,ax
        sub     ax,x1
        jge     @@cu03
        neg     ax
@@cu03:
        mov     bx,x1p
        sub     bx,x1
        jge     @@cu04
        neg     bx
@@cu04:
        cmp     ax,bx
        jge     @@cu05
        mov     ax,x2p
        jmp     excu
@@cu05:
        mov     ax,x1p
        mov     dx,y1p
        jmp     excu
calcu  endp
dline endp

;perevodx  proc near          ;перевод x карты в х окна экрана
;       push  dx           ;вход ax - X карты
;       push  cx           ;выход ах - Х окна
;       push  bx
;       sub   ax,xmindr      ;x должно попадать в окно
;       mov   bx,dxwdr          ;не изменяет регистров
;       mov   cx,dxmdr
;       mov   dx,wxmindr
;       call  calc
;       pop   bx
;       pop   cx
;       pop   dx
;       ret
;perevodx  endp
;perevody  proc near          ;перевод y карты в y окна экрана
;       push  ax           ;вход dx - y карты
;       push  bx           ;выход dх - y окна
;       push  cx
;       mov   ax,ymaxdr      ;y должно попадать в окно
;       sub   ax,dx        ;не изменяет регистров
;       mov   bx,dywdr
;       mov   cx,dymdr
;       mov   dx,wymindr
;       call  calc
;       mov   dx,ax
;       pop   cx
;       pop   bx
;       pop   ax
;       ret
;perevody endp
perevodx  proc near          ;перевод x карты в х окна экрана
        push  dx           ;вход ax - X карты
;       push  cx           ;выход ах - Х окна
;       push  bx
        sub   ax,xmindr      ;x должно попадать в окно
;       imul  dxwdr          ;не изменяет регистров
;       idiv  dxmdr
;       add   ax,wxmindr
        CALCM dxwdr,dxmdr,wxmindr
;       call  calc
;       pop   bx
;       pop   cx
        pop   dx
        ret
perevodx  endp
perevody  proc near          ;перевод y карты в y окна экрана
        push  ax           ;вход dx - y карты
;       push  bx           ;выход dх - y окна
;       push  cx
        mov   ax,ymaxdr      ;y должно попадать в окно
        sub   ax,dx        ;не изменяет регистров
        CALCM dywdr,dymdr,wymindr
;       imul  dywdr
;       idiv  dymdr
;       add   ax,wymindr
;       call  calc
        mov   dx,ax
;       pop   cx
;       pop   bx
        pop   ax
        ret
perevody endp
xmapwin   proc far
        push  bp
        mov   bp,sp
        mov   ax,[bp+6]
        call  perevodx
        pop   bp
        ret   2
xmapwin   endp
ymapwin   proc far
        push  bp
        mov   bp,sp
        mov   dx,[bp+6]
        call  perevody
        mov   ax,dx
        pop   bp
        ret   2
ymapwin   endp
xwinmap   proc far
        push  bp
        mov   bp,sp
        mov   ax,[bp+6]
        sub     ax,wxmindr
;       mov     bx,dxmdr
;       mov     cx,dxwdr
;       mov     dx,xmindr
;       call    calc
        CALCM   dxmdr,dxwdr,xmindr
        pop   bp
        ret   2
xwinmap   endp
ywinmap   proc far
        push  bp
        mov   bp,sp
        mov     ax,wymaxdr      ;y должно попадать в окно
        sub     ax,[bp+6]       ;не изменяет регистров
;       mov     bx,dymdr
;       mov     cx,dywdr
;       mov     dx,ymindr
;       call    calc
        CALCM   dymdr,dywdr,ymindr
        pop   bp
        ret   2
ywinmap   endp
prizram proc near
;ax - x
;dx - y
;выход bl - код
        xor     bl,bl
        cmp     ax,xmindr
        jl      @@pr02
        cmp     ax,xmaxdr
        jg      @@pr03
@@pr00:
        cmp     dx,ymindr
        jl      @@pr04
        cmp     dx,ymaxdr
        jg      @@pr05
@@pr01:
        ret
@@pr02:
        mov     bl,0001b
        jmp     @@pr00
@@pr03:
        mov     bl,0010b
        jmp     @@pr00
@@pr04:
        or      bl,0100b
        ret
@@pr05:
        or      bl,1000b
        ret
prizram endp

newsep proc pascal far arr0:dword
                        ;на входе - far-адрес исходного массива
                        ;выход - в CUR_BUFF_PTR


LOCAL   dx1,dy1:word    ;текущее значение порога по x и y
LOCAL   spsto:word,sisto:word
LOCAL   npoint:word
        mov     spsto,sp
        mov     di,word ptr arr0
        mov     si,word ptr _CUR_BUFF_PTR
        mov     npoint,0
        call    next_point

        mov     lastpr,bl
        or      bl,bl
        jnz     @@ns_fout

@@ns_in:
        call    perevodx
        call    perevody
@@ns_in00:
        call    add_point
        call    next_point
        or      bl,bl
        jz      @@ns_in
@@ns_in_out:
        mov     lastpr,bl
        call    calldl
        mov     ax,x2d
        mov     dx,y2d
        call    add_point
        mov     bl,lastpr
@@ns_fout:
        xor     bh,bh
        cmp     byte ptr datatable[bx],80h
        jb      @@ns_out
        call    add_ugol
@@ns_out:
        call    next_point
        cmp     bl,lastpr
        jz      @@ns_out
        or      bl,bl
        jz      @@ns_out_in
        mov     bh,bl
        and     bh,lastpr
        jz      @@ns_out_calc
        xor     bh,bh
        mov     lastpr,bl
        cmp     byte ptr datatable[bx],80h
        jb      @@ns_out
        call    add_ugol
        jmp     @@ns_out
@@ns_out_in:
        mov     lastpr,bl
        call    calldl
        mov     ax,x1d
        mov     dx,y1d
        call    add_point
        mov     ax,x2d
        mov     dx,y2d
        jmp     @@ns_in00
@@ns_out_calc:
        mov     orpr,bl
        call    calldl
        cmp     byte ptr nt,2
        jne     @@ns_out_calc00
        mov     ax,x1d
        mov     dx,y1d
        call    add_point
        mov     ax,x2d
        mov     dx,y2d
        call    add_point
@@ns_out_calc01:
        mov     bl,orpr
        mov     lastpr,bl
        xor     bh,bh
        cmp     byte ptr datatable[bx],80h
        jb      @@ns_000
        call    add_ugol
@@ns_000:
        jmp     @@ns_out
@@ns_out_calc00:
        mov     bl,orpr
        or      bl,lastpr
        cmp     bl,1111b
        je      @@ns_out_calc02
@@ns_out_calc03:
        mov     bl,orpr
        xor     bh,bh
        mov     al,byte ptr arean[bx]
        shl     al,1
        shl     al,1
        shl     al,1
        mov     bl,lastpr
        add     al,byte ptr arean[bx]
        mov     bl,al
        mov     bl,ugoln[bx]
        call    add_ugol
        jmp     @@ns_out_calc01
@@ns_out_calc02:
        ;AX,DX - содержат последнюю рассчитанную точку.
        ;        меняем ее на x1,y2 и перепроверяем
        call    prizram
        mov     lastpr,bl
        jmp     @@ns_out_calc03
calldl  proc    near
        push    si
        push    di
        mov     es,word ptr arr0[2]
        call    dline pascal,es:[di-8],es:[di-6],es:[di-4],es:[di-2]
        pop     di
        pop     si
        ret
calldl  endp
add_point proc near
        add     ax,_VIEW._LEFT
        add     dx,_VIEW._TOP
@@add_entry:
        mov     es,word ptr _CUR_BUFF_PTR[2]
        cmp     npoint,0
        je      @@ap01
@@ap08:
        cmp     ax,es:[si]
        jne     @@ap02
        cmp     dx,es:[si+2]
        je      @@ap00
@@ap02:
        cmp     npoint,2
        jl      @@ap04
        cmp     byte ptr es:[si+4],0
        jne     @@ap05
@@ap06:
        cmp     ax,es:[si-8]
        jne     @@ap04
        cmp     dx,es:[si-6]
        jne     @@ap04
        sub     si,8
        dec     npoint
        jmp     @@ap00
@@ap04:
        add     si,8
@@ap01:
        mov     es:[si],ax
        mov     es:[si+2],dx
        mov     byte ptr es:[si+4],0
        inc     npoint
        cmp     npoint,1000d
        jge     @@ap03
@@ap00:
        ret
@@ap03:
        mov     sp,spsto
        jmp     @@nw0e
@@ap05:
        cmp     ax,es:[si]
        jne     @@ap07
        cmp     ax,es:[si-8]
        jne     @@ap04
@@ap09:
        sub     si,8
        dec     npoint
        jmp     @@ap08
@@ap07:
        cmp     dx,es:[si+2]
        jne     @@ap10
        cmp     dx,es:[si-6]
        jne     @@ap04
        jmp     @@ap09
@@ap10:
        jmp     @@ap04

add_point endp
add_ugol proc near
;bl - код угла
        mov     ax,npoint
        mov     sisto,ax
        test    bl,0001b
        jz      @@au00
        mov     ax,wxmindr
@@au01:
        test    bl,0100b
        jz      @@au02
        mov     dx,wymaxdr
@@au03:
        call    add_point
        mov     ax,npoint
        cmp     ax,sisto
        jle     @@au04
        mov     byte ptr es:[si+4],bl
@@au04:
        ret
@@au00:
        mov     ax,wxmaxdr
        jmp     @@au01
@@au02:
        mov     dx,wymindr
        jmp     @@au03
add_ugol endp
next_point proc near
        mov  es,word ptr arr0[2]
        mov  ax,es:[di]
        mov  dx,es:[di+2]
        add  di,4
        cmp  ax,0C018h
        je   @@np00
        call prizram
        ret
@@np00:
        pop  ax
next_point endp
@@nw0e:
        les     di,_CUR_BUFF_PTR
        cmp     npoint,3
        jle     @@nw4e
        cmp     byte ptr es:[di+4],0
        je      @@nw1e
        cmp     byte ptr es:[di+12],0
        jne     @@nw1e

        mov     ax,es:[di+8]
        cmp     ax,es:[di]
        jne     @@nw2e
        cmp     ax,es:[si]
        jne     @@nw1e
        cmp     ax,es:[si-8]
        jne     @@nw1e
@@nw6e:
        sub     si,8
        dec     npoint
        mov     ax,es:[si]
        mov     es:[di],ax
        mov     ax,es:[si+2]
        mov     es:[di+2],ax
        jmp     @@nw4e
@@nw2e:
        mov     ax,es:[di+10]
        cmp     ax,es:[di+2]
        jne     @@nw5e
        cmp     ax,es:[si+2]
        jne     @@nw1e
        cmp     ax,es:[si-6]
        jne     @@nw1e
        jmp     @@nw6e
@@nw5e:
        nop
@@nw1e:
        mov     ax,es:[di]
        mov     dx,es:[di+2]
        call    @@add_entry
@@nw4e:
        mov     ax,npoint
        ret
newsep endp

polyline proc pascal far metr:dword
USES    SI,DI
poly_entry:
        call    __preline
        les     si,metr
        add     si,8
        cmp     es:[si],0C018h
        je      pl_end
pl_start:
        cmp     es:[si+4],0C018h
        je      pl_end
        push    es
        push    si
        call    dline   Pascal,es:[si],es:[si+2],es:[si+4],es:[si+6]
        cmp     nt,0
        je      pl_next
        call    LINE2 Pascal,x1d,y1d,x2d,y2d
;       cmp     nt,3
;       je      pl_next
;       mov     si,offset _CUR_LINE_PATTERN
;       mov     di,offset __line_mask_5
;       push    ds
;       pop     es
;       cld
;       mov     cx,5
;       rep     movsw
;       mov     ax,_CUR_LINE_PATTERN
;       mov     __line_mask,ax
pl_next:
        pop     si
        pop     es
        add     si,4
        jmp     short pl_start
pl_end:
        call    __postline
plg_exit:
        ret
polyline endp


POLYGON proc pascal far metr:dword
USES    SI,DI
LOCAL   gortbl:word,ymin:word,ymax:word
LOCAL   npoints:word,curtbl:word
        add     word ptr metr,8
        call    newsep pascal,metr
        cmp     ax,3
        jle     @@pl00
        cmp     ax,1000d
        jge     @@pl00
        dec     ax
        call    fillentry pascal,ax
@@pl00:
        ret
POLYGON ENDP

TEXTLIN  proc  pascal  far  metr : dword,out_arr :dword
USES    si,di
LOCAL   npoint  :word;текущее число точек
        mov     di,word ptr metr
        mov     si,word ptr out_arr
        mov     npoint,0
        call    tlnext_point

        or      bl,bl
        jnz     @@tl_out

@@tl_in:
        call    perevodx
        call    perevody
@@tl_in00:
        call    tladd_point
        call    tlnext_point
        or      bl,bl
        jz      @@tl_in

@@tl_in_out:
        call    tlcalldl
        mov     ax,x2d
        mov     dx,y2d
        call    tladd_point
        jmp     @@tl_exit

@@tl_out:
        mov     lastpr,bl
        call    tlnext_point
        cmp     bl,lastpr
        jz      @@tl_out
        or      bl,bl
        jz      @@tl_out_in
        mov     bh,bl
        and     bh,lastpr
        jz      @@tl_out_calc
        mov     lastpr,bl
        jmp     @@tl_out
@@tl_out_in:
        mov     lastpr,bl
        call    tlcalldl
        mov     ax,x1d
        mov     dx,y1d
        call    tladd_point
        mov     ax,x2d
        mov     dx,y2d
        jmp     @@tl_in00
@@tl_out_calc:
        mov     orpr,bl
        call    tlcalldl
        cmp     byte ptr nt,2
        jne     @@tl_out_calc00
        mov     ax,x1d
        mov     dx,y1d
        call    tladd_point
        mov     ax,x2d
        mov     dx,y2d
        call    tladd_point
        jmp     @@tl_exit
@@tl_out_calc00:
        mov     bl,orpr
        mov     lastpr,bl
        jmp     @@tl_out
tlcalldl proc   near
        push    si
        push    di
        mov     es,word ptr metr[2]
        call    dline pascal,es:[di-8],es:[di-6],es:[di-4],es:[di-2]
        pop     di
        pop     si
        ret
tlcalldl endp
tladd_point proc near
@@tladd_entry:
        mov     es,word ptr out_arr[2]
        cmp     npoint,0
        je      @@tlap01
        cmp     ax,es:[si]
        jne     @@tlap03
        cmp     dx,es:[si+2]
        je      @@tlap00
@@tlap03:
        add     si,4
@@tlap01:
        mov     es:[si],ax
        mov     es:[si+2],dx
        inc     npoint
        cmp     npoint,100d
        jge     @@tlap04
@@tlap00:
        ret
@@tlap04:
        mov     sp,spsto
        jmp     @@tl0e
tladd_point endp
tlnext_point proc near
        mov  es,word ptr metr[2]
        mov  ax,es:[di]
        mov  dx,es:[di+2]
        add  di,4
        cmp  ax,0C018h
        je   @@tlnp00
        call prizram
        ret
@@tlnp00:
        pop  ax
tlnext_point endp
@@tl_exit:
@@tl0e:
        mov     ax,npoint
        ret
TEXTLIN endp

public  dline
public  perevodx
public  perevody
public  SETVIEW
public  SETVIEWMAP
public  XMAPWIN
public  YMAPWIN
public  XWINMAP
public  YWINMAP
public  DRAWLINE
public  POLYLINE
public  POLYGON
public  TEXTLIN


GLOBAL  xmindr  ;габариты окна карты
GLOBAL  ymaxdr
GLOBAL  xmaxdr
GLOBAL  ymindr
GLOBAL  nt      ;возврат из drawline
GLOBAL  x1d
GLOBAL  y1d
GLOBAL  x2d
GLOBAL  y2d
GLOBAL  wymindr
GLOBAL  wymaxdr
GLOBAL  datatable
end