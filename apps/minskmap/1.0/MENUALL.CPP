#include <alloc.h>
#include "dbf.hpp"
#include "fstmod.hpp"
#include "flibrt.hpp"
#include "menuall2.hpp"
#include "tpedit.hpp"
#include "fatr.hpp"

#define max(arg1, arg2) (((arg1) > (arg2)) ? (arg1) : (arg2))
#define min(arg1, arg2) (((arg1) < (arg2)) ? (arg1) : (arg2))

int poisk(int regim, int *x = NULL, int *y = NULL, double *d = NULL);
long poiskdbf(int *x = NULL, int *y = NULL);
int setviewMAP();
int draw_one_obj(int obj, int color);
int printwnd(int xmin, int ymin, int xmax, int ymax, int mask1, int mask2);
int obj_enabled(F_SPR *spr);
void draw_kr();
int M_K_R_status(void (*dr)() = draw_kr, int (*keyhandler)(int) = NULL);
void whatisit(int mode = 0, int seti = 0);
void new_dbf();
void del_dbf();
void act_dbf();
void pnt_dbf();
void red_rec();
void help(const char * help_file);

double XEKRAZD(int y)
{
  return (YMAXMAPE - y) * (XMAXV - XMINV) / YMAXMAPE + XMINV;
};

double YEKRAZD(int x)
{
  return x * (YMAXV - YMINV) / XMAXMAPE + YMINV;
};

int XRAZEKR(float y)
{
  return float((y - YMINV) * (XMAXMAPE + 1) / (YMAXV - YMINV));
};

int YRAZEKR(float x)
{
  return float((XMAXV - x) * (YMAXMAPE + 1) / (XMAXV - XMINV));
};

double XDISRAZ(int y)
{
  return (y - model.fm.ymin) * (XMAXM - XMINM) / (model.fm.ymax - model.fm.ymin) + XMINM;
}

double YDISRAZ(int x)
{
  return (x - model.fm.xmin) * (YMAXM - YMINM) / (model.fm.xmax - model.fm.xmin) + YMINM;
}

int _xMK = 240, _yMK = 240;

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

void draw_kr()
{
  int x = XRAZEKR(_DYM);
  int y = YRAZEKR(_DXM);
  line(0, y, XMAXMAPE, y);
  line(x, 0, x, YMAXMAPE);
}

int M_K_R_status(void (*draw_kur)(), int (*keyhandler)(int))
{
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
  draw_kur();
l0:
  while (1)
  {
    if (xc != x1 || yc != y1)
    {
      double dx = _DXM + (y1 - yc) * XK, dy = _DYM + (xc - x1) * YK;
      cor_dxdy(&dx, &dy);
      if ((dx != _DXM) || (dy != _DYM))
      {
        draw_kur();
        _DXM = dx;
        _DYM = dy;
        draw_kur();
      }
      set_position(xc, yc);
    }
    if (status2 = bioskey(1))
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
  draw_kur();
  _xMK = XRAZEKR(_DYM);
  _yMK = YRAZEKR(_DXM);
  mouse_screen_size(xmn, ymn, xmx, ymx);
  mouse_show(ON);
  setwritemode(COPY_PUT);
  return ret;
}

void restore_otkat()
{
  if (otkatn <= 1)
    return;
  otkatn -= 2;
  memmove(&XMINV, otkat + otkatn * 4, 4 * sizeof(double));
  drawrazd();
}

void cor_V()
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

void fragment(void)
{
  if (CURRENTSCALE < 2.8 * SCALEMIN)
  {
    predproc(1);
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
};

void plmash(void)
{
  if (CURRENTSCALE < 2.8 * SCALEMIN)
  {
    predproc(1);
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
};

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
};

void all(void)
{
  if (All_Map)
    return;
  memmove(&XMINV, &XMING, 4 * sizeof(double));
  drawrazd();
};

void outtxt(int x, int y, char *str)
{
  textsettingstype t;
  SVS svs;
  savesettings(&svs);
  gettextsettings(&t);
  settextstyle(SMALL_FONT, HORIZ_DIR, 4);
  int wd = textwidth(str), ht = textheight(str), x2, y2, h, v;
  if (x + wd < XMAXMAPE)
  {
    h = LEFT_TEXT;
    x2 = x + wd;
  }
  else
  {
    h = RIGHT_TEXT;
    x2 = x - wd;
  }
  if (y + ht < getmaxy())
  {
    v = TOP_TEXT;
    y2 = y + ht - 1;
  }
  else
  {
    v = BOTTOM_TEXT;
    y2 = y - ht + 1;
    y++;
  }
  settextjustify(h, v);
  setfillstyle(SOLID_FILL, WHITE);
  setcolor(BLACK);
  bar(x, y - 2, x2, y2);
  outtextxy(x, y, str);
  settextstyle(t.font, t.direction, t.charsize);
  settextjustify(t.horiz, t.vert);
  restoresettings(&svs);
};

char *ston = "Вкл. N";
char *stoff = "Выкл. N";

void main_menu(char *parst, int parbl)
{
  setcolor(BLACK);
  mouse_show(OFF);
  line(480, 39, 480, 196);
  line(639, 39, 639, 196);
  button b1(480, 49, 559, 64, 1, "Все"),
      b2(560, 49, 639, 64, 2, "Фрагмент"),
      b3(480, 65, 559, 80, 3, "Центр"),
      b4(560, 65, 639, 80, 4, "Возврат"),
      b31(480, 81, 559, 96, 14, "Крупнее"),
      b41(560, 81, 639, 96, 15, "Мельче"),
      b5(480, 97, 559, 112, 5, "Расст-ние"),
      b51(560, 97, 639, 112, 16, ston),
      b6(480, 123, 559, 138, 6, "Что это"),
      b7(560, 123, 639, 138, 7, "Дом"),
      b8(480, 139, 559, 154, 8, "Улица"),
      b9(560, 139, 639, 154, 9, "Район"),
      b91(480, 155, 559, 170, 17, "Печать"),
      b92(560, 155, 639, 170, 18, "Слайд"),
      b10(480, 181, 559, 196, 10, "Вкл/Выкл"),
      b11(560, 181, 639, 196, 11, "Знак"),
      b12(480, 197, 559, 212, 12, "Что здесь"),
      b13(560, 197, 639, 212, 13, "Поиск"),
      b14(480, 213, 559, 228, 19, "Новая"),
      b15(560, 213, 639, 228, 20, "Удал.базу"),
      b16(480, 229, 559, 244, 21, "Назначить"),
      b17(560, 229, 639, 244, 22, "Ввод"),
      b18(480, 245, 559, 260, 23, "Изменение"),
      b19(560, 245, 639, 260, 24, "Уд.объект"),
      b20(480, 263, 559, 278, 25, "Справка"),
      b21(560, 263, 639, 278, -1, "Выход");
  Menu_but menm;
  menm + &b1 + &b2 + &b3 + &b4 + &b31 + &b41 + &b5 + &b51 + &b6 + &b7 + &b8 + &b9 + &b91 + &b92 +
      &b10 + &b11 + &b12 + &b13 + &b14 + &b15 + &b16 + &b17 + &b18 + &b19 + &b20 + &b21;
  menm.draw();
  outtextxy(560, 44, "О б з о р");
  outtextxy(560, 118, "К а р т а");
  outtextxy(562, 176, "Б а з ы");
  mouse_show(ON);
  if (parst)
    if (find_bl(parst, parbl))
      all();
  int keym;
  do
  {
    keym = menm.menu_key2();
    switch (keym)
    {
    case 1:
      all();
      break;
    case 2:
      fragment();
      break;
    case 3:
      centr();
      break;
    case 4:
      restore_otkat();
      break;
    case 14:
      plmash();
      break;
    case 15:
      minmash();
      break;
    case 5:
      rasst();
      break;
    case 16:
    {
      if (N_on)
      {
        N_on = 0;
        b51.text = ston;
      }
      else
      {
        N_on = 1;
        b51.text = stoff;
      }
      menm.draw();
      if (CURRENTSCALE < 12.0 * SCALEMIN)
        drawrazd();
      break;
    }
    case 6:
      whatisit();
      break;
    case 7:
      find_bl();
      break;
    case 8:
      find_st();
      break;
    case 9:
      find_rg();
      break;
    case 17:
      printwnd(0, 0, XMAXMAPE, getmaxy(), 0xaf28, 0x4096);
      break;
    case 18:
      slide();
      break;
    case 10:
      onoff();
      break;
    case 11:
      chmark();
      break;
    case 12:
      whatisitd(0);
      break;
    case 13:
      finddbf();
      break;
    case 19:
      new_dbf();
      break;
    case 20:
      del_dbf();
      break;
    case 21:
      act_dbf();
      break;
    case 22:
      pnt_dbf();
      break;
    case 23:
      red_rec();
      break;
    case 24:
      whatisitd(1);
      break;
    case 25:
      help("M_M.hlp");
      break;
    }
  } while (keym != -1);
}