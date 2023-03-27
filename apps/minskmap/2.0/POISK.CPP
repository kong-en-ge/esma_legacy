#include <string.h>

#include "button.hpp"
#include "dbf.hpp"
#include "geomodel.hpp"
#include "flibrt.hpp"
#include "menuall2.hpp"
//#include "test.hpp"
#include "tpedit.hpp"
#include "fwrite.hpp"
#include "globalda.hpp"
#include "apputils.hpp"
#include "funcs.hpp"

int poisk(CViewContext const& viewContext, int mode, int *x, int *y)
{
  CSearchResultCollector resultCollector(5);
  int nobj, k;
  F_SPR spr;
  double Dp;
  int x1, y1, xc, yc;
  double x2, y2;
  int ret;
  if (mode != 2)
  {
    if (!selectObjectPoint(&x2, &y2))
      return -1;
  }
  ret = 0;
  nobj = 0;
  Dp = CURRENTSCALE > 2000 ? 20 : 40;
  if (mode > 0)
  {
    if (mode == 1)
      Dp = DXDISEKR(1000);
    else
      Dp = DXDISEKR(500);
  }
  while ((ret = next_model(ret, mode)) > 0)
  {
    int nkl = models[ret - 1].numkl;
    if (mode > 0)
    {
      if ((strncmp(flibrt[nkl].clcod, "651", 3) != 0) &&
          (strncmp(flibrt[nkl].clcod, "652", 3) != 0) &&
          (strncmp(flibrt[nkl].clcod, "653", 3) != 0) &&
          (strncmp(flibrt[nkl].clcod, "62132", 5) != 0))
        continue;
    }
    else
    {
      if ((strncmp(flibrt[nkl].clcod, "451", 3) == 0) ||
          (strncmp(flibrt[nkl].clcod, "7131", 4) == 0) ||
          (strncmp(flibrt[nkl].clcod, "71111", 5) == 0))
        continue;
    }

    if (flibrt[nkl].loc > 3)
      continue;
    if (mode != 2)
    {
      yc = YRAZDIS(x2);
      xc = XRAZDIS(y2);
    }
    else
    {
      xc = *x;
      yc = *y;
    }
    double D;
    int d = DXEKRDIS(Dp);
    int d2 = d, l;
    for (unsigned i = model.pm.start; i <= model.pm.end; i++)
    {
      if (model.readspr(&spr, i) != 1)
        continue;
      if (!xydregion(xc, yc, flibrt[spr.numincl].loc == 1 ? d2 : d, (REGION *)&spr.xmin))
        continue;
      if (mode == 0)
      {
        if (!obj_enabled(viewContext, &spr))
          continue;
      }
      if (flibrt[spr.numincl].loc == 1)
      {

        if ((D = length(xc - spr.xmin, yc - spr.ymin)) < d2)
        {
          l = D / 4;
        }
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
      if (mode == 0)
      {
        if (resultCollector.addObject(i, dp, flibrt[spr.numincl].color) && resultCollector.isFull())
        {
          Dp = resultCollector.maxDistance();
          d = DXEKRDIS(Dp);
          d2 = Dp < 2.5 ? 4 * d : DXEKRDIS(10);
        }
        nobj = resultCollector.getCount();
      }
      else
      {
        for (int j = 0; j < nobj; j++)
        {
          if (dp < obstop[j].d)
            break;
        }
        if (j >= 20)
          continue;
        memmove(obstop + j + 1, obstop + j, (nobj - j) * sizeof(OBSTOP));
        obstop[j].d = dp;
        obstop[j].nobj = i;
        if ((strncmp(flibrt[nkl].clcod, "62132", 5)) == 0)
          obstop[j].vt = 1;
        else if ((strncmp(flibrt[nkl].clcod, "651", 3)) == 0)
          obstop[j].vt = 2;
        else if ((strncmp(flibrt[nkl].clcod, "652", 3)) == 0)
          obstop[j].vt = 3;
        else
          obstop[j].vt = 4;
        if (nobj < 20)
          nobj++;
        if (nobj == 20)
        {
          Dp = obstop[19].d;
          d = DXEKRDIS(Dp);
          d2 = Dp < 2.5 ? 4 * d : DXEKRDIS(10);
        }
      }
    } // for
  } // while

  if (nobj != 0)
  {
    if (mode == 0)
    {
      int retValue = chooseObject(viewContext, resultCollector.getObj(), resultCollector.getCount());
      if (retValue >= 0)
      {
        if (x != NULL)
          *x = x1;
        if (y != NULL)
          *y = y1;
      }
      return retValue;
    }   // mode==0
    else // mode > 0
    {
      if (x != NULL)
        *x = xc;
      if (y != NULL)
        *y = yc;
      return nobj;
    }
  }
  else
  {
    if (mode != 2)
      displayWarning(0);
  }

  if (mode == 0)
    return -1;
  else
    return 0;
}
