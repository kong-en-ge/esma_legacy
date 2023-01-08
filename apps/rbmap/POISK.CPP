#include <alloc.h>
#include <string.h>

#include "button.hpp"
#include "dbf.hpp"
#include "geomodel.hpp"
#include "menuall2.hpp"
#include "globals.hpp"
#include "apputils.hpp"
#include "funcs.hpp"

int poisk(int /*mode*/, int *x, int *y)
{
  int nobj, k;
  OBJND obj[6];
  F_SPR spr;
  int x1, y1, ret;
  double Dp, x2, y2;

  if (!selectObjectPoint(&x2, &y2))
    return -1;

  ret = 0;
  nobj = 0;
  Dp = CURRENTSCALE > 2000 ? 20 : 40;
  while ((ret = next_model(ret)) > 0)
  {
    int nkl = models[ret - 1].numkl;
    if (!(strncmp(flibrt[nkl].clcod, "451", 3)) ||
        !(strncmp(flibrt[nkl].clcod, "7131", 4)) ||
        !(strncmp(flibrt[nkl].clcod, "71111", 5)))
      continue;
    if (flibrt[nkl].loc > 3)
      continue;
    int yc = YRAZDIS(x2);
    int xc = XRAZDIS(y2);
    double D;
    int d = DXEKRDIS(Dp);
    int d2 = d, l;
    for (unsigned i = model.pm.start; i <= model.pm.end; i++)
    {
      if (model.readspr(&spr, i) != 1)
        continue;
      if (!xydregion(xc, yc, flibrt[spr.numincl].loc == 1 ? d2 : d, (REGION *)&spr.xmin))
        continue;
      if (!obj_enabled(&spr))
        continue;
      if (flibrt[spr.numincl].loc == 1)
      {
        if ((D = length(xc - spr.xmin, yc - spr.ymin)) < d2)
          l = D / 4;
        else
          continue;
      }
      else
      {
        int *metr;
        if ((model.readmets((int **)&metr, &spr)) == -1)
          continue;
        if (!findDistanceToPolyline(metr, xc, yc, d, l))
          continue;
      }
      double dp = DXDISEKR(l);
      for (int j = 0; j < nobj; j++)
      {
        if (dp < obj[j].d)
          break;
      }
      if (j >= 5)
        continue;
      memmove(obj + j + 1, obj + j, (nobj - j) * sizeof(OBJND));
      obj[j].d = dp;
      obj[j].nobj = i;
      obj[j].color = flibrt[spr.numincl].color;
      if (nobj < 5)
        nobj++;
      if (nobj == 5)
      {
        Dp = obj[4].d;
        d = DXEKRDIS(Dp);
        d2 = Dp < 2.5 ? 4 * d : DXEKRDIS(10);
      }
    } // for
  }   // while

  int retValue = chooseObject(obj, nobj);
  if (retValue >= 0)
  {
    if (x != NULL)
      *x = x1;
    if (y != NULL)
      *y = y1;
  }
  return retValue;
}
