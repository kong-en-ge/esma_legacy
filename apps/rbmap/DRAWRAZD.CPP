#include <math.h>
#include <string.h>
#include <stdio.h>

#include "mouse.hpp"
#include "globalda.hpp"
#include "drawrazd.hpp"
#include "fwrite.hpp"
#include "drawpl.hpp"
#include "button.hpp"
#include "flibrt.hpp"
#include "fstmod.hpp"
#include "set.hpp"
#include "atribut.hpp"
#include "fatr.hpp"
#include "tpedit.hpp"
#include "funcs.hpp"

int control(int);
int next_model(int startpos, int nc = 0);
int next_dbf(int pos, int m2);
int XRAZEKR(float y);
int YRAZEKR(float x);
int setviewMAP();
int DXM;
void draw_one_d(double _x, double _y, int color = -1);
void draw_fld();

void STOPTIME();
void STARTTIME();

int next_model(int pos, int nc)
{
  int ret;
l1:
  if (pos >= nmodels)
    return 0;
  if (nc == 0)
  {
    int nkl = models[pos].numkl;
    if (flibrt[nkl].minw > SCALES || flibrt[nkl].maxw < SCALES)
    {
      pos++;
      goto l1;
    }
  }
  model.pm = models[pos];
  pos++;
  return pos;
}

void drawcram()
{
  putimage(XMINEC, YMINEC, bram, COPY_PUT);
  xminec = XMINEC + (YMINV - YMINVC) / (YMAXVC - YMINVC) * (XMAXEC - XMINEC);
  yminec = YMAXEC - (XMAXV - XMINVC) / (XMAXVC - XMINVC) * (YMAXEC - YMINEC);
  xmaxec = XMINEC + (YMAXV - YMINVC) / (YMAXVC - YMINVC) * (XMAXEC - XMINEC);
  ymaxec = YMAXEC - (XMINV - XMINVC) / (XMAXVC - XMINVC) * (YMAXEC - YMINEC);
  int d = -1;
  // if ((xmaxec-xminec)<64) d=0;
  if ((xmaxec - xminec) <= 16)
    d = (16 - (xmaxec - xminec)) / 2;
  setcolor(BLACK);
  setlinestyle(0, 0, 3);
  rectangle(xminec - d, yminec - d, xmaxec + d, ymaxec + d);
  setlinestyle(0, 0, 1);
}

int obj_enabled(F_SPR *spr)
{
  if (!regionregion((REGION *)&model.xminw, (REGION *)&spr->xmin))
    return 0;
  if (flibrt[spr->numincl].minw > SCALES || flibrt[spr->numincl].maxw < SCALES)
    return 0;
  if (flibrt[spr->numincl].loc != 3)
    return 1;
  if ((spr->xmax - spr->xmin <= DXM) && (spr->ymax - spr->ymin <= DXM))
    return 0;
  return 1;
}

int FULL_MAP; //=1;
int drawrazd(void)
{
#ifdef DEMO
  STOPTIME();
#endif
  FULL_MAP = 1;
  int pos = 0, ret2 = 0;
  mouse_show(OFF);
  setviewport(0, 0, XMAXMAPE, YMAXMAPE, 1);
  clearviewport();
  //  int dxe=XMAXMAPE,dye=YMAXMAPE;
  double dyv = XMAXV - XMINV, dxv = YMAXV - YMINV;
  if ((dxv <= 0) || (dyv <= 0))
    goto l1;
  if (dxv > dyv)
  {
    dyv = dxv;
  }
  else
  {
    dxv = dyv;
  }
  All_Map = !memcmp(&XMINV, &XMING, 4 * sizeof(double));
  XMINV = (XMAXV + XMINV - dxv) / 2;
  XMAXV = XMINV + dxv;
  YMINV = (YMAXV + YMINV - dyv) / 2;
  YMAXV = YMINV + dyv;
  CURRENTSCALE = dyv / 0.18;
  if ((CURRENTSCALE >= SCALEMAX) || (CURRENTSCALE <= SCALEMIN))
  {
    float sc = CURRENTSCALE >= SCALEMAX ? SCALEMAX : SCALEMIN;
    int ss;
    if (CURRENTSCALE >= SCALEMAX)
    {
      sc = SCALEMAX;
      ss = 255;
    }
    else
    {
      sc = SCALEMIN;
      ss = 0;
    }
    SCALES = ss;
    dxv = dxv * sc / CURRENTSCALE;
    XMINV = (XMAXV + XMINV - dxv) / 2.0;
    XMAXV = XMINV + dxv;
    dyv = dyv * sc / CURRENTSCALE;
    YMINV = (YMAXV + YMINV - dxv) / 2.0;
    YMAXV = YMINV + dyv;
    CURRENTSCALE = sc;
  }
  else
    SCALES = log(CURRENTSCALE / SCALEMIN) / SCALEO;
  memmove(&XMINM, &XMING, 4 * sizeof(double));
  setviewMAP();
  pos = 0;
  store_otkat();
  while (((pos = next_model(pos)) > 0) && (!ret2))
  {
    ret2 = drawmap();
  }
  setviewport(0, 0, XMAXMAPE, YMAXMAPE, 1);
  if (ret2)
    goto l1;
  if (LBSsize)
    draw_fld();
l1:
  mouse_show(ON);
  setviewport(0, 0, getmaxx(), getmaxy(), 1);
  if (!GdEGA)
    drawcram();
  FULL_MAP = 0;
#ifdef DEMO
  STARTTIME();
#endif
  return pos;
};

void drawingline(int *metr, int num)
{
  setlinesstyle(fliblin + num);
  polyline(metr);
  setlinestyle(SOLID_LINE, 0, 1);
}

int setviewMAP()
{
  // int XMINE,YMINE,XMAXE,YMAXE;
  if ((XMAXM <= XMINM) || (YMINM >= YMAXM))
    return 0;
  if ((XMAXV - XMINV) < (XMAXM - XMINM) * 0.00001)
    return 0;
  if ((YMAXV - YMINV) < (YMAXM - YMINM) * 0.00001)
    return 0;
  int xminm, yminm, xmaxm, ymaxm;
  if (YMAXM > YMAXV)
  {
    xmaxm = float((YMAXV - YMINM) * (model.fm.xmax - model.fm.xmin) /
                  (YMAXM - YMINM)) +
            model.fm.xmin;
    XMAXE = XMAXMAPE;
  }
  else
  {
    xmaxm = model.fm.xmax;
    XMAXE = XMAXMAPE * (YMAXM - YMINV) / (YMAXV - YMINV);
  };
  if (YMINM < YMINV)
  {
    xminm = float((YMINV - YMINM) * (model.fm.xmax - model.fm.xmin) /
                  (YMAXM - YMINM)) +
            model.fm.xmin;
    XMINE = 0;
  }
  else
  {
    xminm = model.fm.xmin;
    XMINE = XMAXMAPE * (YMINM - YMINV) / (YMAXV - YMINV);
  };
  if (XMAXM > XMAXV)
  {
    ymaxm = float((XMAXV - XMINM) * (model.fm.ymax - model.fm.ymin) /
                  (XMAXM - XMINM)) +
            model.fm.ymin;
    YMINE = 0;
  }
  else
  {
    ymaxm = model.fm.ymax;
    YMINE = YMAXMAPE * (XMAXV - XMAXM) / (XMAXV - XMINV);
  };
  if (XMINM < XMINV)
  {
    yminm = float((XMINV - XMINM) * (model.fm.ymax - model.fm.ymin) /
                  (XMAXM - XMINM)) +
            model.fm.ymin;
    YMAXE = YMAXMAPE;
  }
  else
  {
    yminm = model.fm.ymin;
    YMAXE = YMAXMAPE * (XMAXV - XMINM) / (XMAXV - XMINV);
  };
  if ((XMINE + 1 >= XMAXE) || (YMINE + 1 >= YMAXE))
    return 0;
  setviewport(XMINE, YMINE, XMAXE, YMAXE, 1);
  setviewmap(xminm, yminm, xmaxm, ymaxm);
  setview(0, 0, XMAXE - XMINE, YMAXE - YMINE);
  scr.assign(0, 0, XMAXE - XMINE, YMAXE - YMINE);
  scr.assign_map(xminm, yminm, xmaxm, ymaxm);
  model.xminw = xminm;
  model.yminw = yminm;
  model.xmaxw = xmaxm;
  model.ymaxw = ymaxm;
  asm mov cx, XMAXE
  asm sub cx, XMINE
  asm mov ax, xmaxm
  asm sub ax, xminm
  asm mov dx, 3
  asm mul dx
  asm add cx, 3
  asm div cx
  asm mov DXM,ax
  return 1;
}

int drawmap()
{
  int exp = 0;
  F_SPR spr;
  for (unsigned i = model.pm.start; i <= model.pm.end; i++)
  {
    int key;
#ifndef DEMO
    if ((mouse_key_status(&key, &key)) == -1)
    {
      while (mouse_key_status(&key, &key))
        ;
      exp = -1;
      goto drawex;
    };
#endif
    draw_one_obj(i, -1);
  };
drawex:
  return exp;
};

void dr_pn_n(F_SPR *spr, int *metr, char *clcod, int color, int en, int cod)
{
  if (!en)
  {
    if ((CURRENTSCALE >= 12.0 * SCALEMIN) && (!strncmp(clcod, "412", 3)))
      return;
    if ((CURRENTSCALE >= 5.0 * SCALEMIN) && (!strncmp(clcod, "42", 2)))
      return;
    if ((CURRENTSCALE >= 10.0 * SCALEMIN) && (cod))
      return;
  }
  mk_atrf ma;
  if ((spr->sbh == 0) || (model.readhars((int **)&ma.har, spr) == -1))
    return;
  ma.har += 4;
  int r;
  char namstd[41], namst[41];
  namst[0] = 0;
  do
  {
    r = make_atr(&ma);
    if (!strncmp(ATRN[ma.natr].atr, "09", 2))
      strcpy(namst, ma.zns);
  } while (r == 0);
  if (!strlen(namst))
    return;
  if (!cod)
  {
    strcpy(namstd, namst);
    if (!strncmp(clcod, "41", 2))
      strupr(namstd);
  }
  else
  {
    if (cod == 1)
      strcpy(namstd, "оз. ");
    else
      strcpy(namstd, "вдхр. ");
    strcat(namstd, namst);
  }
  if (color == 12)
    ft.color = 12;
  else if (!cod)
    ft.color = BLACK;
  else
    ft.color = LIGHTBLUE;
  ft.font = 0;
  ft.size = 0;
  ft.gaps = 1;
  int mst[7];
  mst[4] = spr->xmin / 2 + spr->xmax / 2;
  mst[5] = spr->ymin / 2 + spr->ymax / 2;
  mst[6] = 0xc018;
  scr.outstring(namstd, mst, &ft);
}

int draw_one_obj(unsigned obj, int color, int en)
{
  F_SPR spr;
  if (model.readspr(&spr, obj) == -1)
    return -1;
  if ((!en) && (!obj_enabled(&spr)))
    return 0;
  FLIB *flib = flibrt + spr.numincl;
  int loc = flib->loc;
  if (color == -1)
    color = loc == 3 ? flib->colorg : flib->color;
  setcolor(color);
  if (loc == 1)
  {
    int j = flib->num - 1;
    unsigned mask;
    putmarker_16_n(xmapwin(spr.xmin) - ras[j].x,
                   ymapwin(spr.ymin) - ras[j].y, 16, ras[j].obr);
  }
  else if ((loc >= 2) && (loc <= 4))
  {
    // площади и линии и подписи
    int *metr;
    int j;
    if (model.readmets(&metr, &spr) == -1)
      return -1;
    if (FULL_MAP && loc == 3 && flib->num != 12)
    {
      setfillstyle(flib->num, flib->color);
      polygon(metr - 2);
    }
    else if (loc != 4)
    {
      drawingline(metr - 2, loc == 2 ? flib->num : 0);
    }
    else
    {
      char har[61];
      struct MFT
      {
        unsigned font : 3;
        unsigned size : 3;
        unsigned gaps : 2;
      } mft;
      ft.color = color;
      memmove(&mft, &flib->num, 1);
      ft.font = mft.font;
      ft.size = mft.size;
      ft.gaps = mft.gaps;
      scr.outstring((char *)(metr + size_methar(metr) + 1), metr - 2, &ft);
    }
    if ((flib->clcod[0] == '4') && (loc == 3))
      dr_pn_n(&spr, metr, flib->clcod, color, en, 0);
    if (!strncmp(flib->clcod, "31120000", 8))
      dr_pn_n(&spr, metr, flib->clcod, color, en, 1);
    if (!strncmp(flib->clcod, "31131000", 8))
      dr_pn_n(&spr, metr, flib->clcod, color, en, 2);
  }
  return 0;
}

void draw_one_d(int x, int y, int color, int type)
{
  setviewport(0, 0, XMAXMAPE, YMAXMAPE, 1);
  setcolor(color);
  int r = RASsize - 12 + type;
  putmarker_16_n(x - ras[r].x, y - ras[r].y, 16, ras[r].obr);
}

void draw_one_d(double _x, double _y, int color, int type)
{
  int x = XRAZEKR(_y);
  int y = YRAZEKR(_x);
  draw_one_d(x, y, color, type);
}

void draw_fld()
{
  for (int ib = 0; ib < LBSsize; ib++)
  {
    List_base *lbs = LBS + ib;
    if (!lbs->activ)
      continue;
    DBF dbf;
    char dbn[15];
    sprintf(dbn, "%s.DBF", lbs->file);
    long crec = dbf.Use(dbn, 100);
    if (crec <= 0)
      continue;
    int type = lbs->mark[0] - 'A';
    int color = colordbf[lbs->mark[1] - 'A'];
    for (long ir = 0; ir < crec; ir++)
    {
      dbf.Go(ir + 1);
      if (*(dbf.buffer) == '*')
        continue;
      double xd = dbf.F_value(dbf.F_ref("Xcoord"));
      double yd = dbf.F_value(dbf.F_ref("Ycoord"));
      if ((xd <= XMINV) || (xd >= XMAXV) || (yd <= YMINV) || (yd >= YMAXV))
        continue;
      draw_one_d(xd, yd, color, type);
    }
    dbf.Close();
  }
}