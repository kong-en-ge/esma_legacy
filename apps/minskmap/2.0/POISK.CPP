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
#include "drawrazd.hpp"

int next_model(int startpos, int nc = 0, int prp = 0);
int obj_enabled(F_SPR *spr);

struct OBJND
{
  int nobj;
  float d;
  float dl;
  int color;
};

int poisk(int regim, int *x, int *y, double *doub)
{
  int nobj, k;
  OBJND obj[6];
  F_SPR spr;
  double Dp;
  int x1, y1, xc, yc;
  float x2, y2;
  int ret;
  if (regim != 2)
  {
    GRwnd wns(487, 252, 632, 302, "Поиск объекта", LIGHTBLUE, 1, 1);
    wns.text("Укажите точку объекта");
    ret = M_K_R_status();
    wns.restore();
    if (ret == -1)
      goto ex1;
    x2 = _DXM;
    y2 = _DYM;
  }
  ret = 0;
  nobj = 0;
  Dp = CURRENTSCALE > 2000 ? 20 : 40;
  if (regim > 0)
  {
    if (regim == 1)
      Dp = DXDISEKR(1000);
    else
      Dp = DXDISEKR(500);
  }
  while ((ret = next_model(ret, 0, regim)) > 0)
  {
    int spp;
    int nkl = models[ret - 1].numkl;
    if (regim > 0)
    {
      if (((spp = strncmp(flibrt[nkl].clcod, "651", 3)) != 0) &&
          ((spp = strncmp(flibrt[nkl].clcod, "652", 3)) != 0) &&
          ((spp = strncmp(flibrt[nkl].clcod, "653", 3)) != 0) &&
          ((spp = strncmp(flibrt[nkl].clcod, "62132", 5)) != 0))
        goto ex;
    }
    else
    {
      if (((spp = strncmp(flibrt[nkl].clcod, "451", 3)) == 0) ||
          ((spp = strncmp(flibrt[nkl].clcod, "7131", 4)) == 0) ||
          ((spp = strncmp(flibrt[nkl].clcod, "71111", 5)) == 0))
        goto ex;
    }
    {
      if (flibrt[nkl].loc > 3)
        continue;
      if (regim != 2)
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
      double len = 0, l2 = 0, l3 = 0;
      for (unsigned i = model.pm.start; i <= model.pm.end; i++)
      {
        if (model.readspr(&spr, i) != 1)
          continue;
        if (!xydregion(xc, yc, flibrt[spr.numincl].loc == 1 ? d2 : d, (REGION *)&spr.xmin))
          continue;
        if (regim == 0)
        {
          if (!obj_enabled(&spr))
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
          int b = 0;
          if ((model.readmets((int **)&metr, &spr)) == -1)
            continue;
          metr += 2;
          int x2 = *metr++;
          int y2 = *metr++;
          l = d;
          if ((D = length(x2 - xc, y2 - yc)) < l)
          {
            b = 1;
            l = D;
          }
          while (1)
          {
            x1 = x2;
            y1 = y2;
            x2 = *metr++;
            if (x2 == 0xc018)
              break;
            y2 = *metr++;
            if ((l2 = length(x2 - x1, y2 - y1)) == 0)
              continue;
            len = len + l2;
            if (x2 < x1)
            {
              if (x2 - d > xc)
                continue;
              if (x1 < xc - d)
                continue;
            }
            else
            {
              if (x1 - d > xc)
                continue;
              if (x2 < xc - d)
                continue;
            }
            if (y2 < y1)
            {
              if (y2 - d > yc)
                continue;
              if (y1 < yc - d)
                continue;
            }
            else
            {
              if (y1 - d > yc)
                continue;
              if (y2 < yc - d)
                continue;
            }
            float a7 = x2 - x1;
            float b7 = y2 - y1;
            float t7 = (a7 * (xc - x1) + b7 * (yc - y1)) / l2 / l2;
            if ((t7 > 0) && (t7 < 1))
            {
              if ((D = length(x1 + t7 * a7 - xc, y1 + t7 * b7 - yc)) < l)
              {
                b = 1;
                l = D;
                l3 = len + (t7 - 1) * l2;
              }
            }
            else if (t7 >= 1)
            {
              if ((D = length(x2 - xc, y2 - yc)) < l)
              {
                b = 1;
                l = D;
                l3 = len;
              }
            }
          } // просмотр метрики
          if (b == 0)
            continue;
        }
        double dp = DXDISEKR(l);
        if (regim == 0)
        {
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
          obj[j].dl = l3 / (len + 1e-4);
          obj[j].color = flibrt[spr.numincl].color;
          if (nobj < 5)
            nobj++;
          if (nobj == 5)
          {
            Dp = obj[4].d;
            d = DXEKRDIS(Dp);
            d2 = Dp < 2.5 ? 4 * d : DXEKRDIS(10);
          }
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
    }
  ex:
  } // while
  if (nobj != 0)
  {
    if (regim == 0)
    {
      setviewport(XMINE, YMINE, XMAXE, YMAXE, 1);
      for (int i = 0; i < nobj; i++)
      {
        draw_one_obj(obj[i].nobj, 12, 0);
        settwink(12, obj[i].color);
        GRwnd wns(487, 252, 632, 302, "Поиск объекта", LIGHTBLUE, 1, 1);
        wns.text("Этот объект ?");
        button b1(510, 282, 550, 297, 1, "Да"), b2(570, 280, 610, 295, -1, "Heт");
        Menu_but b;
        b + &b1 + &b2;
        int k;
        k = b.menu_key();
        wns.restore();
        if (k == -1)
        {
          nottwink();
          draw_one_obj(obj[i].nobj, -1, 0);
          continue;
        }
        if (x != NULL)
          *x = x1;
        if (y != NULL)
          *y = y1;
        if (doub != NULL)
          *doub = obj[i].dl;
        return obj[i].nobj;
      } // for
    }   // regim==0
    else if (regim > 0)
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
    if (regim != 2)
      displayWarning(0);
  }
  setviewport(0, 0, XMAXMAPE, getmaxy(), 1);
ex1:
  if (regim == 0)
    return -1;
  else
    return 0;
}
