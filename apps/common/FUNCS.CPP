#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <bios.h>
#include <string.h>
#include <math.h>

//#include "funcs.hpp"
#include "globals.hpp"
#include "graphics.h"
#include "files.hpp"
#include "mouse.hpp"
#include "grwnd.hpp"
#include "funcs.hpp"
#include "apputils.hpp"
#include "tpedit.hpp"
#include "drawpl.hpp"
#include "dbf.hpp"
#include "txtpacks.hpp"
#include "fatr.hpp"
#include "textfile.hpp"
#include "dbffunc.hpp"

const unsigned mscur[] = {
    0xffff, 0xffff, 0xffff, 0xfc1f, 0xf3e7, 0xf777, 0xee3b, 0xec9b,
    0xe9cb, 0xec9b, 0xee3b, 0xf777, 0xf3e7, 0xfc1f, 0xffff, 0xffff,
    0x0000, 0x0000, 0x0000, 0x03e0, 0x0c18, 0x0888, 0x1084, 0x1004,
    0x1634, 0x1004, 0x1084, 0x0888, 0x0c18, 0x03e0, 0x0000, 0x0000};

double XEKRAZD(int y)
{
  return (YMAXMAPE - y) * (XMAXV - XMINV) / YMAXMAPE + XMINV;
}

double YEKRAZD(int x)
{
  return x * (YMAXV - YMINV) / XMAXMAPE + YMINV;
}

int XRAZEKR(float y)
{
  return float((y - YMINV) * (XMAXMAPE + 1) / (YMAXV - YMINV));
}

int YRAZEKR(float x)
{
  return float((XMAXV - x) * (YMAXMAPE + 1) / (XMAXV - XMINV));
}

double XDISRAZ(int y)
{
  return (y - model.fm.ymin) * (XMAXM - XMINM) / (model.fm.ymax - model.fm.ymin) + XMINM;
}

double YDISRAZ(int x)
{
  return (x - model.fm.xmin) * (YMAXM - YMINM) / (model.fm.xmax - model.fm.xmin) + YMINM;
}

int XRAZDIS(double y)
{
  return double((y - YMINM) * (model.fm.xmax - model.fm.xmin)) / (YMAXM - YMINM) + model.fm.xmin;
}

int YRAZDIS(double x)
{
  return double((x - XMINM) * (model.fm.ymax - model.fm.ymin)) / (XMAXM - XMINM) + model.fm.ymin;
}

int DXEKRDIS(int dx)
{
  double d = dx * (YMAXV - YMINV) * (model.fm.xmax - model.fm.xmin) / (XMAXMAPE + 1) / (YMAXM - YMINM);
  return d > 32767.0 ? 32767 : d;
}

double DXDISEKR(int dx)
{
  double d = dx * (YMAXM - YMINM) * (XMAXMAPE + 1) / (YMAXV - YMINV) / (model.fm.xmax - model.fm.xmin);
  return d > 32767.0 ? 32767 : d;
}

int next_model(int index, int ignoreScaleFilter)
{
  while (index < nmodels)
  {
    if (!ignoreScaleFilter)
    {
      int classIndex = models[index].numkl;
      if (flibrt[classIndex].minw > SCALES || flibrt[classIndex].maxw < SCALES)
      {
        index++;
        continue;
      }
    }
    model.pm = models[index];
    return ++index;
  }
  return 0;
}


void store_otkat(void)
{
  if (otkatn && (!memcmp(&XMINV, &otkat[(otkatn - 1) * 4], 4 * sizeof(double))))
    return;

  if (otkatn >= MAX_STORED_VIEWS)
  {
    memmove(otkat, otkat + 4, 4 * sizeof(double) * (MAX_STORED_VIEWS - 1));
    otkatn = MAX_STORED_VIEWS - 1;
  }
  memmove(otkat + otkatn * 4, &XMINV, 4 * sizeof(double));
  otkatn++;
}

void restore_otkat(void)
{
  if (otkatn <= 1)
    return;
  otkatn -= 2;
  memmove(&XMINV, otkat + otkatn * 4, 4 * sizeof(double));
  drawrazd();
}

void rasst(void)
{
  mouse_show(OFF);
  struct xy
  {
    int x, y;
  };
  xy t[100];
  char *mous_buf = new char[mouse_size_buff()];
  if (mous_buf != NULL)
    mouse_store(mous_buf);
  mouse_screen_size(XMINE, YMINE, XMAXE, YMAXE);
  mouse_setgraphcursor(8, 8, mscur);
  set_position(XMAXMAPE / 2, YMAXMAPE / 2);
  int status, x1 = -1, x2, y1, y2, col = getcolor();
  double xz1, yz1;
  double xz, yz;
  int cp = 0, kp = 0, vs = 0;
  double lz = 0.0, al = 0.0;
  GRwnd wnd(487, 230, 631, 312, "Расстояние", LIGHTBLUE, 1, 1);
  mouse_show(ON);
  wnd.text("X,м", 1);
  wnd.text("Y,м", 1);
  do
  {
    if ((status = mouse_key_status(&x2, &y2)) == 0)
    {
      if ((x1 != x2) || (y1 != y2))
      {
        char xn[15];
        if (cp)
        {
          mouse_show(OFF);
          setwritemode(XOR_PUT);
          setcolor(BLACK);
          if (x1 != -1)
            line(t[cp].x, t[cp].y, x1, y1);
          line(t[cp].x, t[cp].y, x2, y2);
          setwritemode(COPY_PUT);
          mouse_show(ON);
        }
        sprintf(xn, "%10g", xz = XEKRAZD(y1 = y2));
        wnd.textxy(4, 1, xn);
        sprintf(xn, "%10g", yz = YEKRAZD(x1 = x2));
        wnd.textxy(4, 2, xn);
        if (cp)
        {
          if (!kp)
          {
            wnd.textxy(0, 3, "Текущий отрезок,м");
            kp = 1;
          }
          else
          {
            sprintf(xn, "%9g", lz = length((xz1 - xz), (yz1 - yz)));
            wnd.textxy(4, 4, xn);
            //		if (cp>1) {
            if (!vs)
            {
              wnd.textxy(0, 5, "Всего,м");
              vs = 1;
            }
            else
            {
              sprintf(xn, "%10g", al + lz);
              wnd.textxy(4, 6, xn);
              //			}
            }
          }
        } // cp
      }   // x1!=y1
    }
    else if (status == 1)
    {
      cp++;
      t[cp].x = x1;
      t[cp].y = y1;
      if (cp > 1)
      {
        al = al + lz;
      }
      x1 = -1;
      xz1 = xz;
      yz1 = yz;
    }
  } while ((status != -1) && (cp < 99));
  wnd.restore();
  mouse_show(OFF);
  setwritemode(XOR_PUT);
  for (int i = 1; i < cp; i++)
    line(t[i].x, t[i].y, t[i + 1].x, t[i + 1].y);
  if ((x1 != -1) && (cp))
    line(t[cp].x, t[cp].y, x1, y1);
  setwritemode(COPY_PUT);
  if (mous_buf != NULL)
  {
    mouse_restore(mous_buf);
    delete mous_buf;
  }
  setcolor(col);
  mouse_show(ON);
}

const char Russ[] = {'Ф', 'И', 'С', 'В', 'У', 'А', 'П', 'Р', 'Ш', 'О', 'Л',
                     'Д', 'Ь', 'Т', 'Щ', 'З', 'Й', 'К', 'Ы', 'Е', 'Г', 'М',
                     'Ц', 'Ч', 'Н', 'Я', 'Э', 'Б', 'Ю', 'Ж', 'Х', 'Ъ'};

int upcase(int symb)
{
  symb = toupper(symb);
  if ((symb >= 65) && (symb <= 90))
    symb -= 65;
  else
    switch (symb)
    {
    case 34:
    case 39:
      symb = 26;
      break;
    case 44:
    case 60:
      symb = 27;
      break;
    case 46:
    case 62:
      symb = 28;
      break;
    case 58:
    case 59:
      symb = 29;
      break;
    case 91:
    case 123:
      symb = 30;
      break;
    case 93:
    case 125:
      symb = 31;
      break;
    default:
      return symb;
    }
  return Russ[symb];
}

void cor_V(void)
{
  if (XMINV < XMING)
  {
    XMAXV += XMING - XMINV;
    XMINV = XMING;
  }
  if (YMINV < YMING)
  {
    YMAXV += YMING - YMINV;
    YMINV = YMING;
  }
  if (XMAXV > XMAXG)
  {
    XMINV -= XMAXV - XMAXG;
    XMAXV = XMAXG;
  }
  if (YMAXV > YMAXG)
  {
    YMINV -= YMAXV - YMAXG;
    YMAXV = YMAXG;
  }
}

int setviewMAP(void)
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
  }
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
  }
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
  }
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
  }
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

void all(void)
{
  if (!All_Map)
  {
    memmove(&XMINV, &XMING, 4 * sizeof(double));
    drawrazd();
  }
}

void plmash(void)
{
  if (CURRENTSCALE < 2.8 * SCALEMIN)
  {
    displayWarning(1);
    return;
  }
  double dxm = (XMAXV - XMINV) / 4, dym = (YMAXV - YMINV) / 4;
  XMINV += dxm;
  XMAXV -= dxm;
  YMINV += dym;
  YMAXV -= dym;
  if ((XMAXV - XMINV) < 200)
    XMAXV = XMINV + 200;
  if ((YMAXV - YMINV) < 200)
    YMAXV = YMINV + 200;
  drawrazd();
}

void minmash(void)
{
  if (All_Map)
    return;
  if ((XMAXV - XMINV) > 0.75 * (XMAXG - XMING))
  {
    all();
    return;
  }
  double dxm = (XMAXV - XMINV) / 4, dym = (YMAXV - YMINV) / 4;
  XMINV -= dxm;
  XMAXV += dxm;
  YMINV -= dym;
  YMAXV += dym;
  cor_V();
  drawrazd();
}

int x_fr, y_fr;
void draw_fr()
{
  int x = XRAZEKR(_DYM);
  int y = YRAZEKR(_DXM);
  rectangle(x, y, x_fr, y_fr);
  char lf[10];
  int d;
  setviewport(0, 0, getmaxx(), getmaxy(), 1);
  d = YEKRAZD(x_fr) - _DYM;
  sprintf(lf, "%5u", abs(d));
  outtextxy(570, 277, lf);
  d = XEKRAZD(y_fr) - _DXM;
  sprintf(lf, "%5u", abs(d));
  outtextxy(570, 289, lf);
  setviewport(0, 0, XMAXMAPE, YMAXMAPE, 1);
}

void fragment(void)
{
  if (CURRENTSCALE < 2.8 * SCALEMIN)
  {
    displayWarning(1);
    return;
  }
  double xminm, yminm, dxm, dym;
  textsettingstype tf;
  gettextsettings(&tf);

  GRwnd wnf(487, 252, 632, 302, "Размер фрагмента", LIGHTBLUE, 1, 1);
  wnf.text("(в метрах)");
  wnf.text("по гор.", 1);
  wnf.text("по верт.", 1);
label0:
  if (M_K_R_status() == -1)
  {
    wnf.restore();
    return;
  }
  xminm = _DXM;
  yminm = _DYM;
  x_fr = _xMK;
  y_fr = _yMK;
  if (M_K_R_status(draw_fr) == -1)
    goto label0;
  wnf.restore();
  settextstyle(tf.font, tf.direction, tf.charsize);
  settextjustify(tf.horiz, tf.vert);
  dxm = _DXM - xminm, dym = _DYM - yminm;
  if (dxm < 0)
    dxm = -dxm;
  if (dym < 0)
    dym = -dym;
  if (dym >= dxm)
    dxm = dym;
  if (dxm < 200)
    dxm = 200;
  dxm /= 2;
  float dxc = (_DXM + xminm) / 2;
  XMINV = dxc - dxm;
  XMAXV = dxc + dxm;
  dxc = (_DYM + yminm) / 2;
  YMINV = dxc - dxm;
  YMAXV = dxc + dxm;
  cor_V();
  drawrazd();
};

void centr(void)
{
  if (All_Map)
    return;
  int x1, y1;
  if (M_K_R_status() == -1)
    return;
  double dxm = (XMAXV - XMINV) / 2, dym = (YMAXV - YMINV) / 2;
  XMINV = _DXM - dxm;
  XMAXV = _DXM + dxm;
  YMINV = _DYM - dym;
  YMAXV = _DYM + dym;
  cor_V();
  drawrazd();
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
    draw_one_obj(i, -1, 0 /* drawDisabled */);
  };
drawex:
  return exp;
}

void drawingline(int *metr, int num)
{
  setlinesstyle(fliblin + num);
  polyline(metr);
  setlinestyle(SOLID_LINE, 0, 1);
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

void draw_kr(void)
{
  int x = XRAZEKR(_DYM);
  int y = YRAZEKR(_DXM);
  line(0, y, XMAXMAPE, y);
  line(x, 0, x, YMAXMAPE);
}

void cor_dxdy(double *dx, double *dy)
{
  if (*dx > XMAXV)
    *dx = XMAXV;
  if (*dy > YMAXV)
    *dy = YMAXV;
  if (*dx < XMINV)
    *dx = XMINV;
  if (*dy < YMINV)
    *dy = YMINV;
}

int M_K_R_status(void (*drawCursor)(void), int (*keyhandler)(int))
{                                                                                                 
  if (!drawCursor)
    drawCursor = draw_kr;

  mouse_show(OFF);
  int status = 0, status2, xc, yc, x1, y1, ret = -1, xmn, ymn, xmx, ymx;
  xmn = X_MIN;
  ymn = Y_MIN;
  xmx = X_MAX;
  ymx = Y_MAX;
  mouse_screen_size(0, 0, XMAXMAPE, YMAXMAPE);
  set_position(XMAXMAPE / 2, YMAXMAPE / 2);
  position_status(&status, &xc, &yc);
  x1 = xc, y1 = yc;
  setcolor(DARKGRAY);
  _DXM = XEKRAZD(_yMK);
  _DYM = YEKRAZD(_xMK);
  setwritemode(XOR_PUT);
  double XK = (XEKRAZD(yc) - XEKRAZD(yc - 1));
  double YK = (YEKRAZD(xc) - YEKRAZD(xc + 1));
  cor_dxdy(&_DXM, &_DYM);
  drawCursor();
l0:
  while (1)
  {
    if (xc != x1 || yc != y1)
    {
      double dx = _DXM + (y1 - yc) * XK, dy = _DYM + (xc - x1) * YK;
      cor_dxdy(&dx, &dy);
      if ((dx != _DXM) || (dy != _DYM))
      {
        drawCursor();
        _DXM = dx;
        _DYM = dy;
        drawCursor();
      }
      set_position(xc, yc);
    }
    if ((status2 = bioskey(1)) != 0)
      break;
    position_status(&status, &x1, &y1);
    if (status & 0x3)
      break;
  }
  if (status)
  {
    int x2, y2;
    do
      position_status(&status2, &x2, &y2);
    while (status2);
    if (status & 1)
      ret = 0;
    goto ex;
  }
  else
  {
    status2 = bioskey(0);
    if (keyhandler)
    {
      int key = keyhandler(status2);
      if (key != 0)
      {
        ret = key;
        goto ex;
      }
    }
    status2 = CallRScommand(&status2);
    switch (status2)
    {
    case RSenter:
      ret = 0;
    case RSquit:
      goto ex;
    case RSright:
      x1 += _dec;
      if (x1 < X_MIN)
        x1 = X_MIN;
      break;
    case RSleft:
      x1 -= _dec;
      if (x1 > X_MAX)
        x1 = X_MAX;
      break;
    case RSup:
      y1 -= _dec;
      if (y1 < Y_MIN)
        y1 = Y_MIN;
      break;
    case RSdn:
      y1 += _dec;
      if (y1 > Y_MAX)
        y1 = Y_MAX;
      break;
    }
    goto l0;
  }
ex:
  drawCursor();
  _xMK = XRAZEKR(_DYM);
  _yMK = YRAZEKR(_DXM);
  mouse_screen_size(xmn, ymn, xmx, ymx);
  mouse_show(ON);
  setwritemode(COPY_PUT);
  return ret;
}

void redraw(double x1, double y1, double x2, double y2, int d)
{
  double xmn = x1 - (XMAXG - XMING) / d;
  if (xmn < XMING)
    xmn = XMING;
  double xmx = x2 + (XMAXG - XMING) / d;
  if (xmx > XMAXG)
    xmx = XMAXG;
  double ymn = y1 - (YMAXG - YMING) / d;
  if (ymn < YMING)
    ymn = YMING;
  double ymx = y2 + (YMAXG - YMING) / d;
  if (ymx > YMAXG)
    ymx = YMAXG;
  double dxm = xmx - xmn;
  double dym = ymx - ymn;
  if (dym >= dxm)
    dxm = dym;
  dxm /= 2;
  float dxc = (xmx + xmn) / 2;
  XMINV = dxc - dxm;
  XMAXV = dxc + dxm;
  dxc = (ymx + ymn) / 2;
  YMINV = dxc - dxm;
  YMAXV = dxc + dxm;
  drawrazd();
}

int ATR(TEXT_PACK_MENUS *tp, int obj)
{
  int at = 0, r;
  mk_atrf ma;
  char str[80];
  F_SPR spr;
  int ret2 = model.readspr(&spr, obj);
  if (ret2 != 1)
    tp->add("Ошибка чтения справки объекта");
  else
  {
    sprintf(str, " %s", (char far *)KLSKP[spr.numincl]);
    tp->add(str);
    if (spr.lnh == 0)
      tp->add(" ");
    else if (model.readhars((int **)&ma.har, &spr) == -1)
      tp->add("Ошибка чтения атрибутов");
    else
    {
      at = 1;
      ma.har += 4;
      do
      {
        r = make_atr(&ma);
        //			 if (!(strncmp(flibrt[spr.numincl].clcod,"45",2))&&
        //				!(strncmp(ATRN[ma.natr].atr,"ST",2))) strcpy(str,"Название:");
        //			 else
        sprintf(str, "%s%c", ATRN[ma.natr].name, ':');
        tp->add(str);
        if (ATRN[ma.natr].type == 'K')
          sprintf(str, " %s", ATCD + ATCDP[ATRN[ma.natr].start + ma.znk]);
        else if (ATRN[ma.natr].type == 'T')
          sprintf(str, " %s", ma.zns);
        else
          sprintf(str, " %-10.3g", ma.znn);
        tp->add(str);
      } while (r == 0);
    }
  }
  return at;
}

void displayObjectInfo(int objIndex)
{
  TEXT_PACK_MENUS tp(450, 185, 632, 7, "Что это?", "", 1, 4000, 200, 1, 1);
  if ((tp.TEXT_PACK::text != NULL) && (tp.sm != NULL))
  {
    ATR(&tp, objIndex);
    tp.draw();
    tp.textxy(1, 9, "Esc,Enter - выход");
    tp.menu_key2();
    tp.restore();
    tp.del();
  }
  else
    displayError(0);
}

void whatisit()
{
  int x, y;
  int objIndex = poisk(0 /*mode*/, &x, &y);
  if (objIndex >= 0)
  {
    displayObjectInfo(objIndex);
    nottwink();
    draw_one_obj(objIndex, -1);
  }
  setviewport(0, 0, XMAXMAPE, getmaxy(), 1);
}

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
}
