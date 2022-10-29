#include <dir.h>
#include <dos.h>
#include <stdio.h>
#include <string.h>
#include "geomodel.hpp"
unsigned pascal size_methar(const void *buf) {
	asm les di,buf
	asm mov ax,0xC018
	asm mov cx,0x4000
	asm add di,2
	asm cld
	asm repnz scasw
	asm jne s00
	asm neg cx
	asm add cx,0x4001
return _CX;
s00:
return -1;
}
int pascal isexistmodel(char *path,char *mod) {
  char patht[80];struct ffblk ffblk;
  sprintf(patht,"%s.s%s",path,mod);
  int len=strlen(path)+1;
  if (findfirst(patht,&ffblk,FA_ARCH)==-1) return(-1);
  patht[len]='m';
  if (findfirst(patht,&ffblk,FA_ARCH)==-1) return(-2);
  patht[len]='h';
  if (findfirst(patht,&ffblk,FA_ARCH)==-1) return(-3);
  return 0;
}
