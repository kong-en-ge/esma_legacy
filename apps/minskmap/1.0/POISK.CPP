#include <string.h>

#include "geomodel.hpp"
#include "flibrt.hpp"
#include "menuall2.hpp"
#include "tpedit.hpp"
#include "globalda.hpp"
#include "apputils.hpp"

#define max(arg1, arg2) (((arg1) > (arg2)) ? (arg1) : (arg2))
#define min(arg1, arg2) (((arg1) < (arg2)) ? (arg1) : (arg2))

int next_model(int startpos, int nc = 0);
int draw_one_obj(unsigned obj, int color);
int obj_enabled(F_SPR *spr);
void draw_kr();
int M_K_R_status(void (*dr)() = draw_kr, int (*keyhandler)(int) = NULL);

struct OBJND
{
  int nobj;
  float d;
  float dl;
  int color;
};

int poisk(int /*regim*/, int *x, int *y, double *doub)
{
  int nobj, k;
  OBJND obj[6];
  F_SPR spr;
  double Dp;
  GRwnd wns(487, 252, 632, 302, "Поиск объекта", LIGHTBLUE, 1, 1);
  wns.text("Укажите точку объекта");
  int x1, y1;
  float x2, y2;
  int ret;
  ret = M_K_R_status();
  wns.restore();
  if (ret == -1)
    goto ex1;
  x2 = _DXM;
  y2 = _DYM;
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
    double len = 0, l2 = 0, l3 = 0;
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
    } // for
  }   // while
  if (nobj != 0)
  {
    setviewport(XMINE, YMINE, XMAXE, YMAXE, 1);
    for (int i = 0; i < nobj; i++)
    {
      draw_one_obj(obj[i].nobj, 12);
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
        draw_one_obj(obj[i].nobj, -1);
        continue;
      }
      if (x != NULL)
        *x = x1;
      if (y != NULL)
        *y = y1;
      if (doub != NULL)
        *doub = obj[i].dl;
      return obj[i].nobj;
    }
  }
  else
    displayWarning(0);

  setviewport(0, 0, XMAXMAPE, getmaxy(), 1);
ex1:
  return -1;
}
