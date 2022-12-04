#include <stdlib.h>
#include "grwnd.hpp"
#include "tpedit.hpp"

Sreg::Sreg(int xl, int yt, int xr, int yb, int rm)
{
  left = xl;
  right = xr;
  top = yt;
  bot = yb;
  rem = rm;
  mem = NULL;
}

Sreg::~Sreg()
{
  if ((rem == 1) && (mem != NULL))
    restore();
}

void Sreg::store()
{
  if ((rem) && (mem == NULL))
  {
    SVS svs;
    savesettings(&svs);
    unsigned mems = imagesize(left - 1, top - 1, right + 1, bot + 1);
    if ((mems != 0xFFFF) && ((mem = new char[mems]) != NULL))
      getimage(left - 1, top - 1, right + 1, bot + 1, mem);
    restoresettings(&svs);
  }
}

void Sreg::restore()
{
  char OS = SYNC;
  SYNC = 1;
  if ((rem == 1) && (mem != NULL))
  {
    SVS svs;
    savesettings(&svs);
    putimage(left - 1, top - 1, mem, 0);
    delete mem;
    mem = NULL;
    restoresettings(&svs);
    SYNC = OS;
  }
}

void Sreg::restore(int color)
{
  if ((rem == 1) && (mem != NULL))
    restore();
  else
  {
    SVS svs;
    savesettings(&svs);
    setfillstyle(1, color);
    bar(left - 1, top - 1, right + 1, bot + 1);
    restoresettings(&svs);
  }
}
