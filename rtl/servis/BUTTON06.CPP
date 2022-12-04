#include <bios.h>
#include <stdlib.h>
#include <string.h>
#include "tpedit.hpp"
#include "mouse.hpp"
#include "button.hpp"

VPR_menu::VPR_menu(int _left, int _top, int _right, int _sizep)
{
  left = _left;
  top = _top;
  right = _right;
  bot = _top + _sizep * 12;
  sizec = _sizep;
  activ = 0;
  act = 0;
  maxn = 0;
  offset = 0;
  set_colon(1);
  ncol = 1;
}

void VPR_menu::set_colon(int n)
{
  textwidth = (right - left - 22 - BORD_S) / ::textwidth(" ") / n;
  ncol = n;
  sizep = sizec * n;
}

int VPR_menu::with_col()
{
  return (right - left - 17 - BORD_S - ncol) / ncol;
}

int VPR_menu::xright_col(int n_col)
{
  if (n_col == ncol)
    return right - 18 - BORD_S;
  else
    return xleft_col(n_col) + with_col();
}

int VPR_menu::xleft_col(int n_col)
{
  int xl = left + 1;
  for (int i = 0; i < n_col - 1; i++, xl += with_col() + 2)
    ;
  return xl;
}

int VPR_menu::xright_elv()
{
  return right - 1;
}

int VPR_menu::xleft_elv()
{
  return right - 17;
}

int VPR_menu::str_in_col(int n_col)
{
  int n = maxn;
  for (int i = 1; i < n_col; i++)
  {
    if ((n -= sizec) <= 0)
    {
      n = 0;
      break;
    }
  }
  return n >= sizep ? sizep : n;
}

void VPR_menu::draw_one(int /* num */)
{
}

void VPR_menu::sdwact()
{
  if (activ + act + 1 < maxn)
  {
    if (act + 1 < sizep)
      act++;
    else
      activ++;
  }
}

void VPR_menu::draw_page(void)
{
  offset = 0;
  for (int i = 0; (i < sizep) && (i < maxn); i++)
    draw_one(i);
}

void VPR_menu::go(int num)
{
  if (activ + act == num)
    return;
  if ((activ <= num) && (num <= activ + sizep - 1))
  {
    int n2 = act;
    act = num - activ;
    draw_one(n2);
    draw_one(act);
  }
  else
  {
    activ = num;
    if (activ + sizep >= maxn)
    {
      activ = maxn - sizep;
      act = num - activ;
    }
    else
      act = 0;
    draw_page();
  }
}

void VPR_menu::draw_one_str(char *str, int num)
{
  SVS svs;
  savesettings(&svs);
  int color;
  int color2 = BLACK;
  int e = one_enable(num + activ);
  if (num == act)
  {
    color = LIGHTGRAY;
    if (!e)
      color2 = WHITE;
    draw_elevator();
  }
  else
  {
    color = WHITE;
    if (!e)
      color2 = LIGHTGRAY;
  }
  setfillstyle(1, color);
  setcolor(color2);
  int siz = sizep / ncol;
  int xn = num / siz + 1, yn = num % siz;
  int n = top + yn * 12;
  bar(xleft_col(xn), n, xright_col(xn), n + 11);
  settextjustify(LEFT_TEXT, TOP_TEXT);
  int len = strlen(str);
  if (len <= textwidth)
    offset = 0;
  else if (offset > len - textwidth + 2)
    offset = len - textwidth + 2;
  char buff[80];
  strncpy(buff, str + offset, textwidth);
  buff[textwidth] = '\x0';
  outtextxy(xleft_col(xn) + BORD_S, n + 2, buff);
  restoresettings(&svs);
}

void VPR_menu::draw_elevator(void)
{
  SVS svs;
  savesettings(&svs);
  setfillstyle(1, LIGHTGRAY);
  bar(xleft_elv() + 1, top + 11, xright_elv() - 1, bot - 12);
  int kol;
  if (maxn > 1)
  {
    long k2 = long(activ + act) * (bot - top - 33);
    kol = k2 / (maxn - 1) + top + 11;
  }
  else
    kol = top + 10;
  setfillstyle(1, WHITE);
  bar(xleft_elv() + 3, kol + 2, xright_elv() - 3, kol + 9);
  setcolor(BLACK);
  rectangle(xleft_elv() + 2, kol + 1, xright_elv() - 2, kol + 10);
  restoresettings(&svs);
}

int VPR_menu::menu_key2(void)
{
  if (maxn == 0)
    return -1;
  SVS svs;
  savesettings(&svs);
  int status, xcoor, ycoor, sm, smol, smol2;
  int excod;
  char *mous_buf = new char[mouse_size_buff()];
  if (mous_buf != NULL)
    mouse_store(mous_buf);
  int tpd = top;
  int btd = bot;
  int lfl = xleft_elv(), rtl = xright_elv();
  setcolor(BLACK);
  mouse_screen_size(left + 1, tpd, rtl, btd);
  set_position(right - 30, top + act * 12);
  rectangle(lfl - 1, tpd - 1, rtl + 1, btd);
  for (int i = 1; i <= ncol; i++)
    rectangle(xleft_col(i) - 1, tpd - 1, xright_col(i) + 1, btd);
  int str1[8] = {lfl + 1, tpd + 8, rtl - 1, tpd + 8, lfl + 8, tpd + 1, lfl + 1, tpd + 8};
  int str2[8] = {lfl + 1, btd - 9, rtl - 1, btd - 9, lfl + 8, btd - 2, lfl + 1, btd - 9};
  setfillstyle(1, LIGHTGRAY);
  fillpoly(4, str1);
  fillpoly(4, str2);
  union
  {
    int status2;
    char ch2[2];
  };

  draw_page();
  restoresettings(&svs);
label0:
  smol = 5;
label1:
  do
  {
    position_status(&status, &xcoor, &ycoor);
    status2 = bioskey(1);
    if (status2)
      status2 = key_handler(status2);
  } while (((status & 3) == 0) && (status2 == 0));
  if (status & 2)
  {
    do
      position_status(&status, &xcoor, &ycoor);
    while (status);
    excod = -1;
    goto escexit;
  };
  if (status)
  {
    offset = 0;
    do
    {
      {
        for (int i = 1; i <= ncol; i++)
          if ((xcoor >= xleft_col(i)) && (xcoor <= xright_col(i)) &&
              (ycoor >= tpd) && (ycoor <= btd))
          {
            sm = (ycoor - tpd - 1) / 12;
            if (str_in_col(i) < sm + 1)
              break;
            sm += sizec * (i - 1);
            if ((act == sm) && (smol == 5))
            {
              do
                position_status(&status, &xcoor, &ycoor);
              while (status);
              excod = activ + sm;
              goto escexit;
            };
            smol = 1;
            if (sm + activ >= maxn)
              sm = maxn - 1;
            go(activ + sm);
          };
      }
      if ((xcoor >= lfl) && (xcoor <= rtl))
      {
        if ((ycoor >= tpd) && (ycoor <= tpd + 10))
        {
        smo2:
          smol = 2;
          if (activ + act > 0)
            go(activ + act - 1);
        }
        else if ((ycoor >= btd - 10) && (ycoor <= btd))
        {
        smo4:
          smol = 4;
          if (activ + act + 1 < maxn)
            go(activ + act + 1);
        }
        else if ((ycoor >= tpd + 11) && (ycoor <= btd - 11))
        {
          smol = 3;
        };
      };
      position_status(&status, &xcoor, &ycoor);
    } while (status);
    if (smol == 3)
    {
      long l1 = (maxn - 1);
      int nact = (ycoor - tpd - 11) * l1 / (btd - tpd - 22);
      go(nact);
    };
    goto label0;
  };
  if (status2)
  {
    while (bioskey(1))
      bioskey(0);
    int RSC = CallRScommand(&status2);
    if (ncol > 1)
    {
      if (RSC == RSleft)
        RSC = RSpgup;
      if (RSC == RSright)
        RSC = RSpgdn;
    }
    switch (RSC)
    {
    case RSenter:
      excod = activ + act;
      goto escexit;
    case RSquit:
      excod = -1;
      goto escexit;
    case RSup:
      offset = 0;
      goto smo2;
    case RSdn:
      offset = 0;
      goto smo4;
    case RSpgup:
    {
      int num = activ + act - sizec;
      go(num > 0 ? num : 0);
    }
    break;
    case RSpgdn:
    {
      int num = activ + act + sizec;
      go(num > maxn - 1 ? maxn - 1 : num);
    }
    break;
    case RScpgup:
      go(0);
      break;
    case RScpgdn:
      go(maxn - 1);
      break;
    case RShome:
      if (offset)
        offset = 0;
      draw_one(act);
      goto label0;
    case RSleft:
      if (offset > 0)
      {
        offset--;
        draw_one(act);
      };
      goto label0;
    case RSend:
      offset = 32000;
      draw_one(act);
      goto label0;
    case RSright:
      offset++;
      draw_one(act);
    default:
      goto label0;
    };
    if (offset)
    {
      offset = 0;
      draw_one(act);
    }
    goto label0;
  };
escexit:
  if ((excod != -1) && (!one_enable(excod)))
    goto label0;
  mouse_show(OFF);
  if (mous_buf != NULL)
  {
    mouse_restore(mous_buf);
    delete mous_buf;
  }
  mouse_show(ON);
  return excod;
}

Big_Menu::Big_Menu(int xl, int yt, int xr, int sizep_, char *titl,
                   char *first, int rem, int _fstr, int endstr) : VPR_menu(xl + 2 * BORD_S, yt + 12 + _fstr * 12 - 1, xr - 2 * BORD_S, sizep_),
                                                                  GRwnd(xl, yt, xr, yt + BORD_S - 3 + 12 * (_fstr + sizep_ + endstr + 1), titl, LIGHTBLUE, rem)
{
  first_str = first;
  fstr = _fstr;
}

int Big_Menu::menu_key(void)
{
  if (maxn == 0)
    return -1;
  draw();
  textxy(1, 1, first_str);
  return menu_key2();
}

//возвращает 0 если обработано или key
int VPR_menu::key_handler(int key)
{
  return key;
}

//Возвращает 1 если возможна выборка пункта
int VPR_menu::one_enable(int key)
{
  return s_in(key);
}
