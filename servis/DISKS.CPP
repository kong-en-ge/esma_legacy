#include <dir.h>
#include <dos.h>
#include "disks.hpp"

int pascal is_disk_valid(int drive) {
  if ((drive < 1) || (drive > 'Z'-'A'+1)) return -1;
  int aaxx;char ddrr;
  asm	mov	ax,0x4408
  asm	mov	bx,drive
  asm	int	0x21
  asm   mov     aaxx,ax
  ddrr=drive-1;
  if (aaxx==0)
  {
   asm   mov     dl,ddrr
   asm   mov     ah,0x10
   asm   int     0x13
   asm   mov     aaxx,ax
   if (aaxx!=0) return -1;
  }
  if ((aaxx!=0) && (aaxx!=1)) return -1;
    return _AX;
}

