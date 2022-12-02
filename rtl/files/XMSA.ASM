        .MODEL  LARGE _TEXT
        .CODE
_XMSerror       db      0
XMSinst         dw      0
XMSfunc         dd      0
XMSinstalled proc
        mov     ax,cs:XMSinst
        ret
XMSinstalled endp
XMSerror proc
        xor     ax,ax
        mov     al,byte ptr cs:_XMSerror
        mov     cs:_XMSerror,al
        ret
XMSerror endp
CallXMS proc
        cmp     cs:XMSinst,0
        je      @@ex
        push    ax
        call    dword ptr cs:XMSfunc
        pop     cx
        cmp     ch,8
        jne     @@1
@@2:
        mov     cs:_XMSerror,bl
        ret
@@1:
        cmp     ax,1
        jne     @@2
        mov     cs:_XMSerror,0
@@ex:
        ret
CallXMS endp
StartupXMS proc
        push    ds
        push    di
        push    si
        mov     ax,4300h
        int     2fh
        cmp     al,80h
        jne     @@s1
        mov     cs:XMSinst,1
        mov     ax,4310h
        int     2fh
        mov     word ptr cs:XMSfunc[2],es
        mov     word ptr cs:XMSfunc,bx
        mov     cs:_XMSerror,0
@@s1:
        pop     si
        pop     di
        pop     ds
        ret
StartupXMS endp
DisableXMS proc
        mov     cs:XMSinst,0
        ret
DisableXMS endp

public XMSINSTALLED
public XMSERROR
public STARTUPXMS
public CALLXMS
public DISABLEXMS

END
