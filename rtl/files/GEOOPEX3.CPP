#include <dir.h>
#include <dos.h>
#include <stdio.h>
#include <string.h>

#include "geomodel.hpp"

void gab_ram(const void *metr, SPR *spr)
{
   asm  push  ds         //построение габаритных рамок
   asm  lds   si,metr    //CX-xmin;BX-xmax;DX-ymin;DI-ymax;
   asm  add   si,8
   asm  cld
   asm  lodsw
   asm  mov   cx,ax
   asm  mov   bx,ax
   asm  lodsw
   asm  mov   dx,ax
   asm  mov   di,ax
gbst:
   asm  lodsw
   asm  cmp   ax,0xC018
   asm  je    gben
   asm  cmp   ax,cx
   asm  jge   gb00
   asm  mov   cx,ax
gb00:
   asm  cmp   ax,bx
   asm  jle   gb01
   asm  mov   bx,ax
gb01:
   asm  lodsw
   asm  cmp   ax,dx
   asm  jge   gb02
   asm  mov   dx,ax
gb02:
   asm  cmp   ax,di
   asm  jle   gbst
   asm  mov   di,ax
   asm  jmp   gbst
gben:
   asm  lds   si,spr
   asm  mov   [si+14],cx
   asm  mov   [si+18],bx
   asm  mov   [si+16],dx
   asm  mov   [si+20],di
   asm  pop   ds
}

int GEO_MODEL::writeform(const void *buff)
{
  return hsp.writehead(buff) != 512 ? -1 : 0;
}

int GEO_MODEL::writespr(const SPR *buff)
{
  int nom, ret = 0;
  if ((nom = buff->nom) != 0)
    if (nom <= hsp.getFileLen())
      ret = hsp.write(nom - 1, 1, buff);
    else
    {
      SPR *b;
      if ((b = new SPR[16]) == NULL)
        return -1;
      memset(b, 0, 512);
      while ((nom & 0xfff0) > hsp.getFileLen())
        if ((ret = hsp.write(hsp.getFileLen(), 16, b)) == -1)
          goto l1;
      memmove(b + (nom - 1) % 16, buff, 32);
      ret = hsp.write((nom - 1) & 0xfff0, 16, b);
    l1:
      delete b;
    };

  if (nom == 0)
    return 1;

  if (ret != -1)
    return 0;

  return -1;
}

int GEO_MODEL::writemetsgb(const void *metr, SPR *spr)
{
  gab_ram(metr,spr);
  return writemet_har(metr,&hmt,&spr->stblm,&spr->enblm);
}

int writemet_har(const void *met_har, Files *f,
			 unsigned *fbl, unsigned *ebl)
{
  unsigned bl=f->getFileLen()-2,fre;int *efb,*efb2;
  unsigned db=0;
  unsigned len;
  int *memp,mem;

  if (f->read(bl,bl+1,(void **)&efb)==-1) goto wex01;
  efb2=efb;
  asm  les  di,efb
  asm  mov  cx,0x200
  asm  mov  ax,0xC018
  asm  cld
w00:
  asm  repnz scasw
  asm  je   wne07
  asm  jmp  wex07
wne07:
  asm  cmp  es:[di],ax
  asm  jne  w00
  asm  neg  cx
  asm  add  cx,0x200
  asm  mov  fre,cx
  asm  mov  word ptr efb,di
  asm  mov  word ptr efb[2],es
  if ((len=size_methar(met_har))==0xffff) goto wex07;
  memp=(int *)met_har+len;
  mem=*memp;*memp=0xc018;
  ((int *)met_har)++;
  if (fre+len>512) {
    db=((fre+len-512+511)/512)*2;
    unsigned sd=0;
    if (f->getFileLen()<f->getBufSize()) {
     if (db>f->getBufSize()-f->getFileLen()) sd=db-(f->getBufSize()-f->getFileLen());
    } else {
     sd=db;
    }
    if (sd!=0) {
      memmove(f->buf,f->buf+(f->recsize*sd),(f->getBufSize()-sd)*f->recsize);
      f->start+=sd;
      sd=(f->recsize*sd)/2;
      efb -=sd;
      efb2-=sd;
    }
    f->end+=db;
  }
  memmove(efb, met_har, len * 2);
  if (f->write(bl, 2 + db, efb2) == -1)
    goto wex01;
  *fbl = 1 + ((fre > 256) ? bl + 1 : bl);
  *ebl = 1 + bl + (fre + len - 4) / 256;
  *memp=mem;
  return 0;
wex07:
  return  7;
wex01:
  *memp=mem;
  return -1;
}

int GEO_MODEL::writemetbuff(SPR *spr)
{
  if (hmt.writefrombuf(spr->stblm - 1, spr->enblm - spr->stblm + 1) == -1)
    return -1;
  else
    return 0;
}

int GEO_MODEL::writeharbuff(SPR *spr)
{
  if (hhr.writefrombuf(spr->stblh - 1, 2) == -1)
    return -1;
  else
    return 0;
}

int GEO_MODEL::writemets(const void *metr, SPR *spr)
{
  return writemet_har(metr, &hmt, &spr->stblm, &spr->enblm);
}

int GEO_MODEL::writehars(const void *har, SPR *spr)
{
  unsigned a;
  return writemet_har(har, &hhr, &spr->stblh, &a);
}
