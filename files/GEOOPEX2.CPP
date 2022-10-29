#include <string.h>
#include "geomodel.hpp"
int pascal readmet_har(void **met_har,Files *f,int no,int fbl,int ebl){
   int *fblp;
   if (fbl==0) return 1;
   if (f->read(fbl-1,ebl-1,(void **)&fblp)==-1) return -1;
asm  push  ds
asm  les   di,fblp
asm  mov   cx,256
asm  mov   ax,0c018h
asm  mov   bx,no //номер объекта
asm  push  es
asm  pop   ds
asm  cld
l00:
asm  repnz scasw
asm  jne   short exitbad
asm  cmp   ax,[di]
asm  je    short exitbad
asm  cmp   bx,[di+4]
asm  je    l01      //найден объект
asm  add   di,6
asm  sub   cx,3
asm  jg    short l00 //повторить поиск
asm  jmp   short exitbad
l01:
asm  lds   si,met_har
asm  sub   di,2
asm  mov   [si],di
asm  mov   [si+2],es
asm  pop   ds
	return 0;

exitbad:
asm  pop  ds
	return 7;
};
int pascal GEO_MODEL::readsprb(void **buff,unsigned no){
  SPR *sprp;
  if (hsp.read(no-1,no,(void**)&sprp)==-1) return -1;
  if (sprp->nom==no) {
    *buff=sprp;
    return 0;
  } else if (sprp->nom==0) return 1;else return 7;
}
int pascal GEO_MODEL::readspr(void  *buff,unsigned no){
  SPR *sprp;int ret;
  if ((ret=readsprb((void**)&sprp,no))==0) memmove(buff,sprp,32);
  return ret;
}
int pascal GEO_MODEL::readmet (void **buff,unsigned no) {//Читает из буфера файла
  SPR *sprp;int ret;
  if ((ret=readsprb((void**)&sprp,no))==0) ret=readmets(buff,sprp);
  return ret;
}
int pascal GEO_MODEL::readhar (void **buff,unsigned no) {//Читает из буфера файла
  SPR *sprp;int ret;
  if ((ret=readsprb((void**)&sprp,no))==0) ret=readhars(buff,sprp);
  return ret;
}
int pascal GEO_MODEL::readhars(void **buff,SPR *spr,char *attr) {
  char *at;int ret;
  *(char**)buff=NULL;
  if ((ret=readhars((void**)&at,spr))!=0) return ret;
  int maxlen=strlen(attr);
// найден разделитель, искать  атрибут
asm push  ds
asm lds   si,at
asm add   si,8

l03:
asm {
  les    di,attr
  mov    cx,maxlen
  repz   cmpsb
  jz     l04  //совпадение атрибута
  dec    si
}
l05:
asm {       //несовпадение атрибута
  lodsb
  cmp    al,'/'
  je     l03
  cmp    al,0x18
  jne    l05
  pop    ds
}
return 1;
l04:
asm {
   cmp  byte ptr [si],':'
   jnz  l05
   inc  si
   les  di,buff
   mov  es:[di],si
   mov  es:[di+2],ds
   pop  ds
}
return 0;
}
int pascal GEO_MODEL::readmets(void **metp,SPR *spr) {   //Читает из буфера файла
    return readmet_har(metp,&hmt,spr->nom,spr->stblm,spr->enblm);}
int pascal GEO_MODEL::readhars(void **harp,SPR *spr){   //Читает из буфера файла
    return readmet_har(harp,&hhr,spr->nom,spr->stblh,spr->stblh+1);}
