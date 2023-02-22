#include <io.h>
#include <dir.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>

#include "graphics.h"
#include "flibrt.hpp"
#include "textfile.hpp"
#include "textpack.hpp"
#include "errhand.hpp"

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

int read_tbls(char *name, int type, CErrorHandler* onError)
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
    handleError(onError, 9, name);
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
      handleError(onError, 9, name);
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

int read_class(char const *name, CErrorHandler* onError)
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
    handleError(onError, 9, name);
    return -1;
  }

  TEXT_PACK txp(FLIBsize * 35 + 1, FLIBsize + 4);
  for (int i = 0; i < FLIBsize; i++)
  {
    char str[40];
    if (txt.readstr(str, 34) == -1)
    {
      handleError(onError, 5);
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

int loadClassifier(char const *path, CErrorHandler* onError)
{
  char filename[MAXPATH];

  sprintf(filename, "%sFLIBRT.DAT", path);
  if (read_tbls(filename, 0, onError))
    return -1;

  sprintf(filename, "%sFLIBRT.RAS", path);
  if (read_tbls(filename, 1, onError))
    return -1;

  for (int i = 0; i < RASsize; i++)
    decode_marker(ras[i].obr);

  sprintf(filename, "%sFLIBRT.LIN", path);
  if (read_tbls(filename, 2, onError))
    return -1;

  sprintf(filename, "%sFLIBRT.TXT", path);
  if (read_class(filename, onError))
    return -1;

  sprintf(filename, "%sFLIBRT.ATN", path);
  if (read_tbls(filename, 3, onError))
    return -1;

  sprintf(filename, "%sFLIBRT.ACD", path);
  if (read_tbls(filename, 4, onError))
    return -1;

  return 0;
}
