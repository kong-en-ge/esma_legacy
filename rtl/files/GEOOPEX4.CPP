#include <dir.h>
#include <dos.h>
#include <stdio.h>
#include <string.h>
#include "geomodel.hpp"

#pragma inline

int GEO_MODEL::findhar(FINDHAR *f)
{
 char *start,*atr=f->atr;int _ret,size,sizeatr;
 int numobj=0;
 if (f->blok<hhr.start || f->blok>=hhr.end) {
   if ((_ret=hhr.read(f->blok,f->blok+hhr.getBufSize()-1,(void **)&start))==-1) return -1;
 }
 start=hhr.buf+(f->blok-hhr.start)*hhr.recsize+f->smes;
 size=(hhr.end-f->blok)*hhr.recsize-f->smes;
 sizeatr=strlen(atr);
asm  {
  mov   cx,size
  shr   cx,1
  les   di,start
  inc   di
  and   di,0xfffe
  cld
}
l00:
asm  {
  cmp   cx,0x100
  jg    l08
  jmp   l01
}
l08:
asm {
  mov   ax,0xc018
  repnz scasw
  jne   l01
  cmp   ax,es:[di]
  jne   l06
}
  return 0;          //Ничего не нашли
l06:
asm {
  cmp   word ptr es:[di+4],0
  je    l00
  mov   dx,es:[di+4]
  mov   numobj,dx
  add   di,6
  sub   cx,3
  jmp   l02
}
l01:
asm {
  push  es           //дочтение
  push  di           //сдвиг указателей
  push  cx
  }
  if (hhr.end>=hhr.getFileLen())  {
   if ((int)_CX>=sizeatr) asm {
	pop cx
	pop di
	pop es
	jmp l08
   }
   _ret=-1;
  } else  {
	if ((_ret=hhr.read(hhr.end-2,hhr.end+hhr.getBufSize()-3,(void**)&start))<=2)
	  _ret=-1;else asm {   //дочтение произведено и успешно
	    mov  ax,_ret
	    sub  al,2
	    xchg al,ah
	    pop cx
	    add cx,ax
	    shl ax,1
	    pop di
	    sub di,ax
	    pop es
	    jmp l00
	  }
   }
   asm add  sp,6
   return -1;
l02:
// найден разделитель, искать  атрибут
asm push  ds
asm shl   cx,1
l03:
asm {
  lds    si,atr
  mov    dx,cx
  mov    cx,sizeatr
  repz   cmpsb
  pushf
  sub    cx,sizeatr
  add    cx,dx
  popf
  jz     l04  //совпадение атрибута
  dec    di
  inc    cx
}
l05:
asm {       //несовпадение атрибута
  mov    al,es:[di]
  inc    di
  dec    cx
  cmp    al,'/'
  je     l03
  cmp    al,0x18
  jne    l05
  dec    di
  inc    cx
  shr    cx,1
  pop    ds
  jmp    l00
}
l04:
{
asm cmp  byte ptr es:[di],':'
asm jz   l09
asm cmp  byte ptr es:[di],'/'
asm jnz  l05
l09:
 int n,n1;
 asm inc  di
 asm dec  cx
 asm pop  ds      //Атрибут найден
 asm push di
 char near *bf=(char near *)hhr.buf;
 n1=hhr.start;
asm {
 pop  ax
 sub  ax,bf
 push ax
 and  ax,0x1ff
 mov  n,ax
 pop  ax
 xchg al,ah
 shr  al,1
 xor  ah,ah
 add  ax,n1
 mov  n1,ax
}
 f->smes=n;
 f->blok=n1;
  return numobj;
}
}
