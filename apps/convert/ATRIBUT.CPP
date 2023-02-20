#include "atribut.hpp"

#pragma inline

// получает текущий атрибут в структуру m_a
// возвращает 0 - o'k, -1 - error ,1 - этот атрибут последний
int make_atr(mk_atr *m_a)
{
  char *har = m_a->har;
  char *atr = m_a->atr;
  char *zna = m_a->zna;
make_atr:
  asm {
   push	ds
   les	di,atr
   lds  si,har
   cld
   xor   bx,bx
  }
make_atr_loop:
  asm {
   lodsb
   cmp  al,':'
   jz   make_zn
   cmp  al,0x18
   jz   l_error
   stosb
   inc  bx
   cmp  bl,2
   jle  make_atr_loop
   jmp  l_error
  }
make_zn:
  asm {
   xor  al,al
   stosb
   xor  bx,bx
   les  di,zna
  }
make_zn_loop:
  asm {
   lodsb
   cmp	al,'/'
   je   end_zn
   cmp  al,0x18
   je   l_error
   stosb
   inc  bx
   cmp  bx,MAX_ATR_SIZE
   jl   make_zn_loop
   jmp  l_error
  }
end_zn:
  asm {
   xor  al,al
   stosb
   mov  cx,1
   mov  al,[si]
   cmp  al,0x18
   je   l_well
   cmp  al,' '
   je   l_well
   xor  cx,cx
      // mov  word ptr har,si
  }
l_well:
  asm pop ds
  (int) m_a->har = _SI;
  asm mov ax, cx;
  asm jmp l_end
l_error:
  asm pop ds
  asm mov ax, -1
l_end:
  return _AX;
}
