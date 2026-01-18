#include <stdio.h>
#include <string.h>

#include "funcs.hpp"
#include "transp.hpp"

void ATRTR(TEXT_PACK_MENUS *tp, CStopArray const &stops)
{
  int r;
  mk_atrf ma;
  F_SPR spr;
  char vidtr[15];
  strcpy(vidtr, "  Метро:");
  int k = 1, ck, fl = 0;
  char mr[55], nst[40];
  do
  {
    fl = 0;
    for (ck = 0; ck < stops.getCount(); ck++)
    {
      if (stops[ck].vt != k)
        continue;

      if (!loadAttributes(stops[ck].nobj, ma))
        continue;

      mr[0] = 0;
      nst[0] = 0;
      do
      {
        r = make_atr(&ma);
        if (k != 1)
        {
          if (checkAttributeDef(ma.natr, "NM", 'T'))
            sprintf(mr, " %s", ma.zns);
        }
        if (checkAttributeDef(ma.natr, "09", 'T'))
          if (k != 1)
            sprintf(nst, "Ост. %s", ma.zns);
          else
            sprintf(nst, "Станция %s", ma.zns);
      } while (r == 0);

      if (((fl == 0) && (strlen(mr) != 0) && (k != 1)) ||
          ((fl == 0) && (strlen(nst) != 0) && (k == 1)))
      {
        tp->add(vidtr);
        fl = 1;
      }
      if (strlen(mr) != 0)
        tp->add(mr);
      if (strlen(nst) != 0)
        tp->add(nst);
    }

    k++;
    if (k == 2)
      strcpy(vidtr, "  Автобусы:");
    if (k == 3)
      strcpy(vidtr, "  Троллейбусы:");
    if (k == 4)
      strcpy(vidtr, "  Трамваи:");
  } while (k <= 4);
}
