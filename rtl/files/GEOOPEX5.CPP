#include <dir.h>
#include <dos.h>
#include <stdio.h>
#include <string.h>
#include "geomodel.hpp"
#pragma inline
int pascal GEO_MODEL::readhars(SPR *spr,char *attr,char *str,int n) {
int ret;char *harp;
  if ((ret=readhars((void **)&harp,spr,attr))==0) {
asm {
    push ds
    lds  si,harp
    les  di,str
    mov  cx,n
    cld
}
l00:
asm {
    lodsb
    cmp  al,'/'
    jz   l01
    cmp  al,0x18
    jz   l01
    stosb
    loop l00
}
l01:
asm {
    mov  byte ptr es:[di],0
    pop  ds
  }
  }
  return ret;
}