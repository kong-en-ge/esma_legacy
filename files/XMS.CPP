#include "xms.h"

extern "C" {
  void pascal CallXMS(void);
  void pascal StartupXMS(void);
}

#pragma startup  StartupXMS

void getXMSmem(unsigned int *Total,unsigned int *MaxBlock) {
   _AH=8;
   CallXMS();
   *Total=_DX;
   *MaxBlock=_AX;
}

unsigned int allocXMS(unsigned int Mem) {
   _DX=Mem;
   _AH=9;
   CallXMS();
   return _AX==1 ? _DX : 0;
}

int freeXMS(unsigned int  handle) {
   _DX=handle;
   _AH=0x0a;
   CallXMS();
   return (_AX);
}

int moveXMS(EMMSTRUC far *M) {
  asm {
    push        ds
    push        si
    lds         si,M
    mov         ah,0xb
  }
  CallXMS();
  asm   pop     si
  asm   pop     ds
  return (_AX);
}

void getXMSmem(unsigned int *Total,unsigned int *MaxBlock);
unsigned int allocXMS(unsigned int Mem);
int freeXMS(unsigned int  handle);
int moveXMS(EMMSTRUC far *M);
