#include <bios.h>
#include <dos.h>
#include <memory.h>
#include <string.h>

#include "graphics.h"
#include "mouse.hpp"
#include "tpedit.hpp"
#include "ctype.h"

int Inserting = 1, CursorToEnd = 1, ForceUpper = 0;
int NCOL, COL1, COL2;
extern pascal char ISCOL1;
void write_cursor();
void DrawString(char *St);
void Insert(char ch, char *St);
#define beep() \
  sound(200);  \
  delay(50);   \
  nosound()

extern "C"
{
  void pascal setvec8(void far *);
  int pascal restore8(void);
  void pascal twinkle(void);
}
void settwink(int ncol, int num)
{
  NCOL = ncol;
  struct palettetype colors;
  getpalette(&colors);
  COL1 = colors.colors[ncol];
  COL2 = colors.colors[num];
  ISCOL1 = 0;
  setvec8(twinkle);
}
void nottwink(void)
{
  if (restore8())
    setpalette(NCOL, COL1);
};

int XStr, YStr, Sp, MaxStr, AttrStr, Len, Offs, Flag = 1;
int Cursor = 0;
char SYNC = 0;
void write_cursor()
{
  if (Flag)
    return;
  asm cli
      mouse_show(OFF);
  int fon = (AttrStr & 0xf0) >> 4;
  int attr = 0x0f & AttrStr;
  int xStr = XStr + Sp * 8;
  char bitmap[40];
  int y = YStr + (Inserting ? 7 : 0);
  getimage(xStr, y, xStr + 7, YStr + 8, bitmap);
  for (int j = 0; j < 9; j++)
  {
    int mask = 8;
    char *c = bitmap + (j + 1) * 4;
    for (int i = 0; i < 4; c++, i++)
      if ((mask >> i) & (fon ^ attr))
        *c = ~(*c);
  }
  putimage(xStr, y, bitmap, 0);
  Cursor = !Cursor;
  mouse_show(ON);
  asm sti
}

int pascal CallRScommand(int *Chword)
{
  union
  {
    int chword;
    unsigned char ch[2];
  };
  chword = *Chword;
  switch (ch[0])
  {
  case 0:
    switch (ch[1])
    {
    case 71:
      return RShome;
    case 75:
      return RSleft;
    case 77:
      return RSright;
    case 72:
      return RSup;
    case 80:
      return RSdn;
    case 79:
      return RSend;
    case 82:
      return RSins;
    case 83:
      return RSdel;
    case 73:
      return RSpgup;
    case 81:
      return RSpgdn;
    case 132:
      return RScpgup;
    case 118:
      return RScpgdn;
    default:
      return RSnone;
    }
  case 13:
    return RSenter;
  case 255:
    return RSquit;
  case 27:
    return RSquit;
  case '\b':
    return RSback;
  default:
    if ((ch[0] >= 32) && (ch[0] < 255u))
    {
      if (ForceUpper)
      {
        ch[0] = Uppercase(ch[0]);
        *Chword = chword;
      };
      return RSchar;
    }
    else
      return RSnone;
  }
}

void Insert(char ch, char *St)
{
  int strl = strlen(St);
  char *S = St + strl;
  for (strl -= Sp + Offs; strl >= 0; strl--, S--)
    *(S + 1) = *S;
  *(St + Sp + Offs) = ch;
}

void DrawString(char *St)
{
  mouse_show(OFF);
  Cursor = 0;
  int len = strlen(St + Offs);
  char *s = St + Offs + MaxStr + 1, s2 = *s;
  *s = 0;
  char lef = St[Offs];
  if (Offs)
    St[Offs] = 0x11;
  char rig = *(s - 1);
  if (len > MaxStr + 1)
    *(s - 1) = 0x10;
  bar(XStr, YStr, XStr + 8 * MaxStr + 7, YStr + 8);
  outtextxy(XStr, YStr, St + Offs);
  *s = s2;
  St[Offs] = lef;
  *(s - 1) = rig;
  write_cursor();
  mouse_show(ON);
};

int ReadString(int Row, int Col, int MaxLen, int StringAttr, char *Str, int MaxLenEk)
{
  mouse_show(OFF);
  if (!MaxLenEk)
    MaxLenEk = MaxLen;
  XStr = Row;
  YStr = Col;
  MaxStr = MaxLenEk - 1;
  AttrStr = StringAttr;
  settextjustify(LEFT_TEXT, TOP_TEXT);
  char St[256];
  int FirstChar = 1, RSC, Escap, Done;
  Escap = Done = 0;
  union
  {
    int ChWord;
    char Ch2[2];
  };
  setcolor(AttrStr & 15);
  setfillstyle(1, (AttrStr & 0xf0) >> 4);

  strcpy(St, Str);
  Len = strlen(St);
  Offs = 0;
  if (Len > MaxLen)
  {
    *(St + MaxLen) = '\0';
    Len = MaxLen;
  }
  Sp = CursorToEnd ? Len : Sp = 0;
  if (Sp >= MaxLenEk)
  {
    Sp = MaxStr;
    Offs = Len - MaxLenEk;
  }
  setvec8(write_cursor);
  do
  { //
    DrawString(St);
    Flag = 0;
    int status, xcoor, ycoor;
    do
    {
      //    position_status(&status,&xcoor,&ycoor);
      ChWord = bioskey(1);
    } while (/*((status & 3)==0) && */ (ChWord == 0));
    Flag = 1;
    /*  if (status & 2) {
        do position_status(&status,&xcoor,&ycoor); while (status);
        RSC=RSquit;
        goto label1;
      };
      if (status & 1) {
            do {
              if ((xcoor>=Row) && (xcoor<=Row+MaxLenEk*8) &&
                     (ycoor>=Col) && (ycoor<=Col+10)) {
                      int col=(xcoor-Row)/8;
                      if (Cursor) write_cursor();
                      if (col<(Sp=strlen(St)-1)) Sp=col;
              };
              position_status(&status,&xcoor,&ycoor);
              RSC=RSnone;
            } while(status);
            goto label1;
            }
    */
    ChWord = bioskey(0);

    RSC = CallRScommand(&ChWord);
  label1:
    if (FirstChar)
    {
      FirstChar = 0;
      if (RSC == RSchar)
      {
        *St = 0;
        Sp = 0;
        Offs = 0;
      }
    }

    Len = strlen(St);
    setmem(St + Len, MaxLen - Len + 1, 0);
    switch (RSC)
    {
    case RSchar:
      if (Inserting)
      {
        if (Len < MaxLen)
        {
          Insert(Ch2[0], St);
          goto right;
        }
        else
        {
          beep();
        }
      }
      else
      { // Not Inserting
        St[Sp + Offs] = Ch2[0];
        goto right;
      }
      break;
    case RSenter:
      Done = 1;
      break;
    case RSquit:
      strcpy(St, Str);
      if (strlen(St) > MaxStr)
        St[MaxLen] = 0;
      Done = 1;
      Escap = 1;
      DrawString(St);
      break;
    case RShome:
      Sp = 0;
      Offs = 0;
      break;
    case RSend:
      Offs = (Len > MaxStr) ? Len - MaxStr : 0;
      Sp = Len - Offs;
      if (Len == MaxLen)
        Offs--;
      break;
    left:
    case RSleft:
      if (Sp > 0)
        Sp--;
      else if (Offs > 0)
        Offs--;
      else
        goto def;
      break;
    right:
      Len = strlen(St);
    case RSright:
      if (Sp + Offs < Len)
      {
        if (Sp < MaxStr)
          Sp++;
        else if (Offs + Sp < MaxLen - 1)
          Offs++;
        else
          goto def;
      }
      else
        goto def;
      break;
    case RSins:
      Inserting = !Inserting;
      break;
    case RSdel:
      if (Len && (Sp + Offs < Len))
      {
        strcpy(St + Sp + Offs, St + Sp + Offs + 1);
        if (Offs + MaxStr >= Len && Offs > 0)
          Offs--;
        if (Sp + Offs > Len && Sp > 0)
          Sp--;
      }
      else
        goto def;
      break;
    case RSback:
      if (Sp + Offs)
      {
        Len--;
        strcpy(St + Sp + Offs - 1, St + Sp + Offs);
        if (Offs + MaxStr >= Len && Offs > 0)
          Offs--;
        else if (Sp > 0)
          Sp--;
      }
      else
        goto def;
      break;
    def:
    default:
      beep();
      break;
    } // switch
    //  if (Sp==MaxStr) Sp--;
  } while (!Done);
  restore8();
  Flag = 0;
  if (Cursor)
    write_cursor();
  Flag = 1;
  strcpy(Str, St);
  while (bioskey(1))
    bioskey(0);
  mouse_show(ON);
  return Escap;
} // ReadString

char *strncpyz(char *dest, const char *sours, int maxlen)
{
  asm push ds
  asm les di, dest
  asm lds si, sours
  asm mov cx, maxlen
  asm cld
l0:
  asm lodsb
  asm or al, al
  asm jz l1
  asm stosb
  asm loop l0
l1:
  asm xor al, al
  asm stosb
  asm pop ds

  return dest;
}

char *delprb(char *str)
{
  int len = strlen(str);
  for (int i = 0; i < len; i++)
    if (str[i] != ' ')
      break;
  for (int j = len - 1; j > i; j--)
    if (str[j] != ' ')
      break;
  if (j < i)
    str[0] = 0;
  else
  {
    str[j + 1] = 0;
    strcpy(str, str + i);
  }
  return str;
}
