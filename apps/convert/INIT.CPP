#include <io.h>
#include <fcntl.h>
#include "convert.hpp"

FLIB *flibrt;
unsigned int *flibsort;
RAS *ras;
linesstyle *fliblin;
unsigned LINsize;
unsigned FLIBsize;
unsigned int *KLSKP; // смещение строк класс
char *KLSK;          // строки класс
atrname *ATRN;
unsigned int ATRsize; // число возможных атрибутов
unsigned int RASsize;
char *ATCD; // Расшифровка кодовых атрибутов
char SYSPATH[MAXPATH];

void decode_marker(unsigned int mar[16])
{
  int buf[16];
  asm push ds
  asm push ss
  asm pop es
  asm lds si, mar
  asm lea di, buf
  asm mov cx, 16
  asm cld
  asm rep movsw
  asm push ds
  asm push es
  asm pop ds
  asm pop es
  asm mov cx, 16
  asm lea si, buf
  asm mov di, word ptr mar
l0:
  asm lodsw
  asm xchg al, ah
  asm mov es:[di + 16], ah
  asm stosb
  asm loop l0
  asm pop ds
}

int flibsortcmp(int *c1, int *c2)
{
  return memcmp(flibrt + (*c1), flibrt + (*c2), 9);
}

int read_tbls(char *name, int type)
{
  int handle;
  int ret = -1;
  unsigned len, flen, size;
  char *buf;
  switch (type)
  {
  case 0:
    size = sizeof(FLIB);
    delete flibrt;
    delete flibsort;
    flibsort = NULL;
    flibrt = NULL;
    break;
  case 1:
    size = sizeof(RAS);
    delete ras;
    ras = NULL;
    break;
  case 2:
    size = sizeof(linesstyle);
    delete fliblin;
    fliblin = NULL;
    break;
  case 3:
    size = sizeof(atrname);
    delete ATRN;
    ATRN = NULL;
    break;
  case 4:
    size = 1;
    delete ATCD;
    ATCD = NULL;
    break;
  }
  if ((handle = open(name, O_BINARY | O_RDONLY)) == -1)
  {
    errorproc(9, name);
    return -1;
  }
  flen = filelength(handle) / size;
  len = 0xff00 / size;
  if (len > flen)
    len = flen;
  int k = 0;
  if (type == 0)
    k = 3;
  buf = new char[(len + k) * size];
  if (read(handle, buf, size * len) == -1)
    goto exl1;
  ret = 0;
  switch (type)
  {
  case 0:
    flibrt = (FLIB *)buf;
    FLIBsize = --len;
    if (memcmp(flibrt, "ESMA2.00                         ", sizeof(FLIB)))
    {
      errorproc(9, name);
      break;
    }
    memmove(flibrt, flibrt + 1, len * sizeof(FLIB));
    flibsort = new unsigned int[FLIBsize];
    for (int i = 0; i < FLIBsize; i++)
      flibsort[i] = i;
    qsort(flibsort, FLIBsize, 2,
          (int (*)(const void *, const void *))flibsortcmp);
    break;
  case 1:
    ras = (RAS *)buf;
    RASsize = len;
    break;
  case 2:
    fliblin = (linesstyle *)buf;
    LINsize = len;
    break;
  case 3:
    ATRN = (atrname *)buf;
    ATRsize = len;
    break;
  case 4:
    ATCD = buf;
    break;
  }
exl1:
  close(handle);
  return ret;
}

int read_klas(char const *name)
{
  TEXTFILE txt;
  if (KLSK)
    delete KLSK;
  KLSK = NULL;
  if (KLSKP)
    delete KLSKP;
  KLSKP = NULL;
  if (txt.open(name, 10230) == -1)
  {
    errorproc(9, name);
    return -1;
  }
  TEXT_PACK txp(FLIBsize * 35 + 1, FLIBsize + 4);
  for (int i = 0; i < FLIBsize; i++)
  {
    char str[40];
    if (txt.readstr(str, 34) == -1)
    {
      errorproc(5);
      return -1;
    }
    if (txt.status() == T_EOF)
      break;
    txp.add(str);
  }
  for (; i < FLIBsize; i++)
    txp.add("");
  txp.store_to(&KLSK, (int **)&KLSKP);
  return 0;
}

int Init()
{
  //   strcpy(SYSPATH,_argv[0]);
  //   *(strrchr(SYSPATH,'\\')+1)=0;
  SYSPATH[0] = '\\';
  getcurdir(0, SYSPATH + 1);
  strcat(SYSPATH, "\\");
  int gd, gm;
  detectgraph(&gd, &gm); // gm=1;
  if ((gd != EGA) && (gd != VGA))
    gd = VGA;
  initgraph(&gd, &gm, NULL);
  gm = graphresult();
  if (gm != grOk)
    return -1;
  if (gd == VGA)
  {
    static unsigned char arr[][4] = {
        {EGA_BLACK, 0x2A, 0x2A, 0x2A},
        {EGA_BLUE, 0, 0, 0x2D},
        {EGA_GREEN, 0x2, 0x2A, 0x2},
        {EGA_RED, 0x2A, 2, 2},
        {EGA_BROWN, 0x2A, 0x15, 0},
        {EGA_LIGHTGRAY, 0x1D, 0x1D, 0x1D},
        {EGA_DARKGRAY, 0x4, 0x4, 0x4},
        {EGA_LIGHTBLUE, 0x15, 0x15, 0x3F},
        {EGA_LIGHTGREEN, 0x2, 0x2A, 0x2},
        {EGA_LIGHTCYAN, 0x20, 0x3F, 0x3F},
        {EGA_LIGHTRED, 0x3F, 0x15, 0},
        {EGA_LIGHTMAGENTA, 0x3F, 0x23, 0x16},
        {EGA_YELLOW, 0x3A, 0x3A, 0x13},
        {EGA_WHITE, 0, 0, 0}};
    for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++)
      setrgbpalette(arr[i][0], arr[i][1], arr[i][2], arr[i][3]);
  }
  else
  {
    setpalette(15, 0);
    setpalette(0, 55);
    setpalette(1, 25);
    setpalette(2, 34);
    setpalette(3, 35);
    setpalette(4, 36);
    setpalette(6, 38);
  }
  setfillstyle(1, BLACK);
  bar(0, 0, getmaxx(), getmaxy());
  setcolor(BLACK);
  init_mouse();
  asm mov ax, 0xf
  asm mov cx, 10
  asm mov dx, 8
  asm int 0x33
  mouse_show(ON);
  if (farcoreleft() <= 0x20000l)
  {
    errorproc(7);
    return -1;
  }
  char path[MAXPATH];
  sprintf(path, "%sFLIBRT.DAT", SYSPATH);
  if (read_tbls(path, 0))
    return -1;
  sprintf(path, "%sFLIBRT.RAS", SYSPATH);
  if (read_tbls(path, 1))
    return -1;
  for (int i = 0; i < RASsize; i++)
    decode_marker(ras[i].obr);
  sprintf(path, "%sFLIBRT.LIN", SYSPATH);
  if (read_tbls(path, 2))
    return -1;
  sprintf(path, "%sFLIBRT.TXT", SYSPATH);
  if (read_klas(path))
    return -1;
  sprintf(path, "%sFLIBRT.ATN", SYSPATH);
  if (read_tbls(path, 3))
    return -1;
  sprintf(path, "%sFLIBRT.ACD", SYSPATH);
  if (read_tbls(path, 4))
    return -1;
  return 0;
}
