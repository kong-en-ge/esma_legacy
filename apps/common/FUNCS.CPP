#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <bios.h>
#include <string.h>
#include <math.h>

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
#include "graphapp.hpp"

const unsigned mscur[] = {
    0xffff, 0xffff, 0xffff, 0xfc1f, 0xf3e7, 0xf777, 0xee3b, 0xec9b,
    0xe9cb, 0xec9b, 0xee3b, 0xf777, 0xf3e7, 0xfc1f, 0xffff, 0xffff,
    0x0000, 0x0000, 0x0000, 0x03e0, 0x0c18, 0x0888, 0x1084, 0x1004,
    0x1634, 0x1004, 0x1084, 0x0888, 0x0c18, 0x03e0, 0x0000, 0x0000};

double XEKRAZD(int y)
{
  return (YMAXMAPE - y) * boundsV.getWidth() / YMAXMAPE + boundsV.left;
}

double YEKRAZD(int x)
{
  return x * boundsV.getHeight() / XMAXMAPE + boundsV.top;
}

int XRAZEKR(float y)
{
  return float((y - boundsV.top) * (XMAXMAPE + 1) / boundsV.getHeight());
}

int YRAZEKR(float x)
{
  return float((boundsV.right - x) * (YMAXMAPE + 1) / boundsV.getWidth());
}

double XDISRAZ(int y)
{
  return (y - model.fm.ymin) * boundsM.getWidth() / (model.fm.ymax - model.fm.ymin) + boundsM.left;
}

double YDISRAZ(int x)
{
  return (x - model.fm.xmin) * boundsM.getHeight() / (model.fm.xmax - model.fm.xmin) + boundsM.top;
}

int XRAZDIS(double y)
{
  return double((y - boundsM.top) * (model.fm.xmax - model.fm.xmin)) / boundsM.getHeight() + model.fm.xmin;
}

int YRAZDIS(double x)
{
  return double((x - boundsM.left) * (model.fm.ymax - model.fm.ymin)) / boundsM.getWidth() + model.fm.ymin;
}

int DXEKRDIS(int dx)
{
  double d = dx * boundsV.getHeight() * (model.fm.xmax - model.fm.xmin) / (XMAXMAPE + 1) / boundsM.getHeight();
  return d > 32767.0 ? 32767 : d;
}

double DXDISEKR(int dx)
{
  double d = dx * boundsM.getHeight() * (XMAXMAPE + 1) / boundsV.getHeight() / (model.fm.xmax - model.fm.xmin);
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
  if (otkatn && (!memcmp(&boundsV, &otkat[otkatn - 1], sizeof(CRectD))))
    return;

  if (otkatn >= MAX_STORED_VIEWS)
  {
    memmove(otkat, otkat + 1, sizeof(CRectD) * (MAX_STORED_VIEWS - 1));
    otkatn = MAX_STORED_VIEWS - 1;
  }
  otkat[otkatn] = boundsV;
  otkatn++;
}

void restore_otkat(void)
{
  if (otkatn <= 1)
    return;
  otkatn -= 4;
  boundsV = otkat[otkatn];
  drawrazd();
}

int selectObjectPoint(double *x, double *y)
{
  int retValue;
  GRwnd infoWindow(487, 252, 632, 302, "Поиск объекта", LIGHTBLUE, 1, 1);
  infoWindow.text("Укажите точку объекта");
  if (M_K_R_status() != -1)
  {
    *x = _DXM;
    *y = _DYM;
    retValue = 1;
  }
  else
    retValue = 0;

  infoWindow.restore();
  return retValue;
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
  mouse_screen_size(boundsE.left, boundsE.top, boundsE.right, boundsE.bottom);
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
  double delta;

  delta = boundsG.left - boundsV.left;
  if (delta > 0)
    boundsV.offset(delta, 0);

  delta = boundsG.top - boundsV.top;
  if (delta > 0)
    boundsV.offset(0, delta);

  delta = boundsG.right - boundsV.right;
  if (delta < 0)
    boundsV.offset(delta, 0);

  delta = boundsG.bottom - boundsV.bottom;
  if (delta < 0)
    boundsV.offset(delta, 0);
}

#define MAX_VIEW_RATIO 0.00001

int setviewMAP(void)
{
  // int XMINE,YMINE,XMAXE,YMAXE;
  if ((boundsM.right <= boundsM.left) || (boundsM.top >= boundsM.bottom))
    return 0;
  if (boundsV.getWidth() < boundsM.getWidth() * MAX_VIEW_RATIO)
    return 0;
  if (boundsV.getHeight() < boundsM.getHeight() * MAX_VIEW_RATIO)
    return 0;
  int xminm, yminm, xmaxm, ymaxm;
  if (boundsM.bottom > boundsV.bottom)
  {
    xmaxm = float((boundsV.bottom - boundsM.top) * (model.fm.xmax - model.fm.xmin) /
                  boundsM.getHeight()) +
            model.fm.xmin;
    boundsE.right = XMAXMAPE;
  }
  else
  {
    xmaxm = model.fm.xmax;
    boundsE.right = XMAXMAPE * (boundsM.bottom - boundsV.top) / (boundsV.getHeight());
  }
  if (boundsM.top < boundsV.top)
  {
    xminm = float((boundsV.top - boundsM.top) * (model.fm.xmax - model.fm.xmin) /
                  boundsM.getHeight()) +
            model.fm.xmin;
    boundsE.left = 0;
  }
  else
  {
    xminm = model.fm.xmin;
    boundsE.left = XMAXMAPE * (boundsM.top - boundsV.top) / boundsV.getHeight();
  }
  if (boundsM.right > boundsV.right)
  {
    ymaxm = float((boundsV.right - boundsM.left) * (model.fm.ymax - model.fm.ymin) /
                  boundsM.getWidth()) +
            model.fm.ymin;
    boundsE.top = 0;
  }
  else
  {
    ymaxm = model.fm.ymax;
    boundsE.top = YMAXMAPE * (boundsV.right - boundsM.right) / boundsV.getWidth();
  }
  if (boundsM.left < boundsV.left)
  {
    yminm = float((boundsV.left - boundsM.left) * (model.fm.ymax - model.fm.ymin) /
                  boundsM.getWidth()) +
            model.fm.ymin;
    boundsE.bottom = YMAXMAPE;
  }
  else
  {
    yminm = model.fm.ymin;
    boundsE.bottom = YMAXMAPE * (boundsV.right - boundsM.left) / boundsV.getWidth();
  }
  if ((boundsE.left >= boundsE.right) || (boundsE.top >= boundsE.bottom))
    return 0;

  setviewport(boundsE.left, boundsE.top, boundsE.right, boundsE.bottom, 1);
  setviewmap(xminm, yminm, xmaxm, ymaxm);
  setview(0, 0, boundsE.getWidth(), boundsE.getHeight());
  scr.assign(0, 0, boundsE.getWidth(), boundsE.getHeight());
  scr.assign_map(xminm, yminm, xmaxm, ymaxm);
  model.xminw = xminm;
  model.yminw = yminm;
  model.xmaxw = xmaxm;
  model.ymaxw = ymaxm;
  asm mov cx, word ptr boundsE[4];//.right
  asm sub cx, word ptr boundsE[0];//.left
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
    boundsV = boundsG;
    drawrazd();
  }
}

#define MIN_VIEW_SIZE 200

void plmash(void)
{
  if (CURRENTSCALE < 2.8 * SCALEMIN)
  {
    displayWarning(1);
    return;
  }
  double dxm = boundsV.getWidth() / 4, dym = boundsV.getHeight() / 4;
  boundsV.inflate(-dxm, -dym);
  if (boundsV.getWidth() < MIN_VIEW_SIZE)
    boundsV.right = boundsV.left + MIN_VIEW_SIZE;
  if (boundsV.getHeight() < MIN_VIEW_SIZE)
    boundsV.bottom = boundsV.top + MIN_VIEW_SIZE;
  drawrazd();
}

void minmash(void)
{
  if (All_Map)
    return;
  if (boundsV.getWidth() > 0.75 * boundsG.getWidth())
  {
    all();
    return;
  }
  double dxm = boundsV.getWidth() / 4, dym = boundsV.getHeight() / 4;
  boundsV.inflate(dxm, dym);
  cor_V();
  drawrazd();
}

struct DrawFrameContext {
  int x;
  int y;
};

void draw_fr(void const *drawContext)
{
  int x = XRAZEKR(_DYM);
  int y = YRAZEKR(_DXM);
  DrawFrameContext const* frameContext = (DrawFrameContext const*)drawContext;
  rectangle(x, y, frameContext->x, frameContext->y);
  char lf[10];
  int d;
  setviewport(0, 0, getmaxx(), getmaxy(), 1);
  d = YEKRAZD(frameContext->x) - _DYM;
  sprintf(lf, "%5u", abs(d));
  outtextxy(570, 277, lf);
  d = XEKRAZD(frameContext->y) - _DXM;
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

  DrawFrameContext frameContext;
  do
  {
    if (M_K_R_status() == -1)
    {
      wnf.restore();
      return;
    }
    xminm = _DXM;
    yminm = _DYM;
    frameContext.x = _xMK;
    frameContext.y = _yMK;
  }
  while (M_K_R_status(draw_fr, NULL, &frameContext) == -1);

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
  boundsV.left = dxc - dxm;
  boundsV.right = dxc + dxm;
  dxc = (_DYM + yminm) / 2;
  boundsV.top = dxc - dxm;
  boundsV.bottom = dxc + dxm;
  cor_V();
  drawrazd();
}

void centr(void)
{
  if (All_Map)
    return;
  int x1, y1;
  if (M_K_R_status() == -1)
    return;
  double dxm = boundsV.getWidth() / 2, dym = boundsV.getHeight() / 2;
  boundsV.initByPoint(_DXM, _DYM);
  boundsV.inflate(dxm, dym);
  cor_V();
  drawrazd();
}

int drawmap(void)
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
      break;
    }
#endif
    draw_one_obj(i, -1, 0 /* drawDisabled */);
  }
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

void draw_kr(const void *)
{
  int x = XRAZEKR(_DYM);
  int y = YRAZEKR(_DXM);
  line(0, y, XMAXMAPE, y);
  line(x, 0, x, YMAXMAPE);
}

void cor_dxdy(double *dx, double *dy)
{
  boundsV.putPointInside(dx, dy);
}

int M_K_R_status(DrawCursorFunc drawCursor, int (*keyhandler)(int),
  const void *drawContext)
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
  drawCursor(drawContext);
l0:
  while (1)
  {
    if (xc != x1 || yc != y1)
    {
      double dx = _DXM + (y1 - yc) * XK, dy = _DYM + (xc - x1) * YK;
      cor_dxdy(&dx, &dy);
      if ((dx != _DXM) || (dy != _DYM))
      {
        drawCursor(drawContext);
        _DXM = dx;
        _DYM = dy;
        drawCursor(drawContext);
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
  drawCursor(drawContext);
  _xMK = XRAZEKR(_DYM);
  _yMK = YRAZEKR(_DXM);
  mouse_screen_size(xmn, ymn, xmx, ymx);
  mouse_show(ON);
  setwritemode(COPY_PUT);
  return ret;
}

void redraw(double x1, double y1, double x2, double y2, int d)
{
  double xmn = x1 - boundsG.getWidth() / d;
  if (xmn < boundsG.left)
    xmn = boundsG.left;
  double xmx = x2 + boundsG.getWidth() / d;
  if (xmx > boundsG.right)
    xmx = boundsG.right;
  double ymn = y1 - boundsG.getHeight() / d;
  if (ymn < boundsG.top)
    ymn = boundsG.top;
  double ymx = y2 + boundsG.getHeight() / d;
  if (ymx > boundsG.bottom)
    ymx = boundsG.bottom;
  double dxm = xmx - xmn;
  double dym = ymx - ymn;
  if (dym >= dxm)
    dxm = dym;
  dxm /= 2;
  float dxc = (xmx + xmn) / 2;
  boundsV.left = dxc - dxm;
  boundsV.right = dxc + dxm;
  dxc = (ymx + ymn) / 2;
  boundsV.top = dxc - dxm;
  boundsV.bottom = dxc + dxm;
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
  int objIndex = poisk(0 /*mode*/);
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
  double dyv = boundsV.getWidth(), dxv = boundsV.getHeight();
  if ((dxv <= 0) || (dyv <= 0))
    goto l1;
  if (dxv > dyv)
    dyv = dxv;
  else
    dxv = dyv;
  All_Map = !memcmp(&boundsV, &boundsG, sizeof(CRectD));
  boundsV.left = (boundsV.left + boundsV.right - dxv) / 2;
  boundsV.right = boundsV.left + dxv;
  boundsV.top = (boundsV.top + boundsV.bottom - dyv) / 2;
  boundsV.bottom = boundsV.top + dyv;
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
    boundsV.left = (boundsV.right + boundsV.left - dxv) / 2.0;
    boundsV.right = boundsV.left + dxv;
    dyv = dyv * sc / CURRENTSCALE;
    boundsV.top = (boundsV.bottom + boundsV.top - dxv) / 2.0;
    boundsV.bottom = boundsV.top + dyv;
    CURRENTSCALE = sc;
  }
  else
    SCALES = log(CURRENTSCALE / SCALEMIN) / SCALEO;
  boundsM = boundsG;
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
  if (!CGraphicApplication::getInstance()->isModeEGA())
    drawcram();
  FULL_MAP = 0;
#ifdef DEMO
  STARTTIME();
#endif
  return pos;
}

int obj_enabled(F_SPR const *spr)
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

int chooseObject(const OBJND *obj, int nobj)
{
  int retValue = -1;

  if (nobj > 0)
  {
    setviewport(boundsE.left, boundsE.top, boundsE.right, boundsE.bottom, 1);
    for (int i = 0; i < nobj; i++)
    {
      draw_one_obj(obj[i].nobj, 12);
      settwink(12, obj[i].color);
      GRwnd wndHelper(487, 252, 632, 302, "Поиск объекта", LIGHTBLUE, 1, 1);
      wndHelper.text("Этот объект ?");
      button b1(510, 282, 550, 297, 1, "Да"), b2(570, 280, 610, 295, -1, "Heт");
      Menu_but b;
      b + &b1 + &b2;
      int k;
      k = b.menu_key();
      wndHelper.restore();
      if (k == -1)
      {
        nottwink();
        draw_one_obj(obj[i].nobj, -1);
        continue;
      }
      retValue = obj[i].nobj;
      break;
    }
    setviewport(0, 0, XMAXMAPE, getmaxy(), 1);
  }
  else
    displayWarning(0);

  return retValue;
}

int checkAttributeName(int index, char const* name)
{
  atrname const* atrn = ATRN + index;
  return !strncmp(atrn->atr, name, 2);
}

int checkAttributeDef(int index, char const* name, char type)
{
  atrname const* atrn = ATRN + index;
  return (atrn->type == type) && checkAttributeName(index, name);
}

int getAttributeIndex(char const* name)
{
  for (int i = 0; i < ATRsize; i++)
    if (checkAttributeName(i, name))
      return i;

  return -1;
}

char const * getLookupAttributeValue(int attrIndex, int valueIndex)
{
  return ATCD + ATCDP[ATRN[attrIndex].start + valueIndex];
}

int findDistanceToPolyline(int const* metr, int xc, int yc, int d, int &l)
{
  int x1, y1;
  double D, len = 0, l2 = 0;
  int b = 0;

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
      }
    }
    else if (t7 >= 1)
    {
      if ((D = length(x2 - xc, y2 - yc)) < l)
      {
        b = 1;
        l = D;
      }
    }
  } // просмотр метрики

  if (b == 0)
    return 0;

  return 1;
}

/**************************
 * CSearchResultCollector *
 **************************/

CSearchResultCollector::CSearchResultCollector(int maxCount)
{
  _maxCount = maxCount;
  _count = 0;
  obj = new OBJND[maxCount];
}

CSearchResultCollector::~CSearchResultCollector(void)
{
  delete[] obj;
}

bool CSearchResultCollector::addObject(int index, double distance, int color)
{
  for (int i = 0; i < _count; i++)
  {
    if (distance < obj[i].d)
      break;
  }

  if (i >= _maxCount)
    return false;

  if (_count > i)
    memmove(obj + i + 1, obj + i, (_count - i) * sizeof(OBJND));

  obj[i].d = distance;
  obj[i].nobj = index;
  obj[i].color = color;

  if (_count < _maxCount)
    _count++;

  return true;
}

bool CSearchResultCollector::isFull(void) const
{
  return _count == _maxCount;
}

double CSearchResultCollector::maxDistance(void) const
{
  if (_count > 0)
    return obj[_count - 1].d;
  else
    return 0;
}

