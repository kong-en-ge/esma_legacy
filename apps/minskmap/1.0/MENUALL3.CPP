#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <dir.h>

#include "globalda.hpp"
#include "flibrt.hpp"
#include "txtpacks.hpp"
#include "drawpl.hpp"
#include "fatr.hpp"
#include "menuall2.hpp"
#include "apputils.hpp"
#include "funcs.hpp"

FLIB *findobj(SPR *spr);
int findobjn(SPR *spr);
int poisk(int regim, int *x = NULL, int *y = NULL, double *b = NULL);
int ATR(TEXT_PACK_MENUS *tp, int obj);
int obj_enabled(F_SPR *spr);

struct KLSt : public Big_Menu
{
  int nchar, numat;
  char cod[21];
  void draw_one(int num);
  int key_handler(int key);
  int numinkl(char *cod, int sim);
  KLSt(int ia);
};

KLSt::KLSt(int ia) : Big_Menu(150, 100, 470, 10, "Выбор улицы",
                              "", 1)
{
  maxn = ATRN[ia].ncod;
  textwidth -= 2;
  nchar = 0;
  numat = ia;
}

void KLSt::draw_one(int num)
{
  draw_one_str(ATCD + ATCDP[ATRN[numat].start + activ + num], num);
}

int KLSt::numinkl(char *cod, int sim)
{
  for (int i = 0; i < maxn; i++)
    if (!strncmpi(cod, ATCD + ATCDP[ATRN[numat].start + i], sim))
      return i;
  return -1;
}

int KLSt::key_handler(int key)
{
  int p, k2 = key, num = -1;
  if ((p = CallRScommand(&k2)) != RSchar && p != RSback)
    return key;
  if (p == RSchar)
  {
    if (nchar < 20)
    {
      cod[nchar] = upcase(key);
      if ((num = numinkl(cod, nchar + 1)) >= 0)
        nchar++;
    }
  }
  else
  {
    if (nchar > 0)
      nchar--;
    goto l1;
  }
  if (num == -1)
    return key;
  go(num);
l1:
  char str[21];
  str[20] = 0;
  memset(str, ' ', 20);
  strncpy(str, cod, nchar);
  textxy(2, 1, str);
  return key;
}

void find_rg(void)
{
  FINDatrf fa;
  fa.natr = 1;
  for (int ia = 0; ia < ATRsize; ia++)
    if (!strncmp(ATRN[ia].atr, "09", 2))
      break;
  if (ia == ATRsize)
    return;
  fa.atr[0].natr = ia;
  {
    TEXT_PACK_MENUS tpr(150, 100, 470, 9, "Район", "Укажите район", 1, 200, 10);
    for (int i = 0; i < FLIBsize; i++)
      if (!strncmp(flibrt[i].clcod, "46", 2))
        tpr.add((char far *)KLSKP[i]);
    i = tpr.menu_key();
    if (i != -1)
      strcpy(fa.atr[0].zns, tpr.TEXT_PACK::text + tpr.sm[i]);
    tpr.restore();
    if (i == -1)
      return;
  }
  char *sp = strchr(fa.atr[0].zns, ' ');
  *sp = 0;
  fa.blok = model.fm.sbh;
  fa.smes = model.fm.ssh;
  unsigned ret, count = 0;
  do
  {
    ret = findobj(model, fa);
    if (ret == 0xffff)
      break;
    F_SPR spr;
    if (model.readspr(&spr, ret) != 1)
      break;
    if (strncmp(flibrt[spr.numincl].clcod, "46", 2))
      continue;
    count++;
    if (!All_Map)
      all();
    setviewport(XMINE, YMINE, XMAXE, YMAXE, 1);
    draw_one_obj(ret, 12);
    settwink(12, WHITE);
    {
      int at;
      char str[30];
      TEXT_PACK_MENUS tp(450, 185, 632, 7, "Район",
                         "", 1, 4000, 200, 1, 1);
      if ((tp.TEXT_PACK::text == NULL) || (tp.sm == NULL))
      {
        displayError(0);
        return;
      }
      at = ATR(&tp, ret);
      tp.draw();
      tp.textxy(1, 9, "Enter,Esc - выход");
      at = tp.menu_key2();
      tp.restore();
      nottwink();
      draw_one_obj(ret, -1);
      setviewport(0, 0, XMAXMAPE, getmaxy(), 1);
      tp.del();
      ret = at;
    }
  } while (ret < 0xff00);
  if (count == 0)
    displayWarning(2);
  return;
}

void find_st(void)
{
  textsettingstype t;
  gettextsettings(&t);
  FINDatrf fa;
  fa.natr = 1;
  for (int ia = 0; ia < ATRsize; ia++)
    if (!strncmp(ATRN[ia].atr, "ST", 2))
      break;
  if (ia == ATRsize)
    return;
  fa.atr[0].natr = ia;
  {
    KLSt mn(ia);
    mn.draw();
    int key = mn.menu_key2();
    if (key == -1)
      return;
    fa.atr[0].znk = key;
  }
  fa.blok = model.fm.sbh;
  fa.smes = model.fm.ssh;
  unsigned ret, count = 0;
  do
  {
    ret = findobj(model, fa);
    if (ret == 0xffff)
      break;
    F_SPR spr;
    if (model.readspr(&spr, ret) != 1)
      break;
    if ((strncmp(flibrt[spr.numincl].clcod, "452", 3)) && (strncmp(flibrt[spr.numincl].clcod, "453", 3)) && (strncmp(flibrt[spr.numincl].clcod, "450", 3)))
      continue;
    count++;
    int dx = xwinmap(30) - xwinmap(0);
    if ((!obj_enabled(&spr)) || (spr.xmin < model.xminw) ||
        (spr.ymin < model.yminw) || (spr.xmax > (model.xmaxw - dx)) ||
        (spr.ymax > model.ymaxw))
    {
      settextjustify(t.horiz, t.vert);
      redraw(XDISRAZ(spr.ymin), YDISRAZ(spr.xmin), XDISRAZ(spr.ymax),
             YDISRAZ(spr.xmax), 24);
    }
    setviewport(XMINE, YMINE, XMAXE, YMAXE, 1);
    draw_one_obj(ret, 12);
    settwink(12, WHITE);
    {
      int at;
      char str[30];
      TEXT_PACK_MENUS tp(450, 185, 632, 7, "Улица", "", 1, 4000, 200, 1, 1);
      if ((tp.TEXT_PACK::text == NULL) || (tp.sm == NULL))
      {
        displayError(0);
        return;
      }
      at = ATR(&tp, ret);
      tp.draw();
      tp.textxy(1, 9, "Enter-еще,Esc-выход");
      at = tp.menu_key2();
      tp.restore();
      nottwink();
      draw_one_obj(ret, -1);
      setviewport(0, 0, XMAXMAPE, getmaxy(), 1);
      tp.del();
      ret = at;
    }
  } while (ret < 0xff00);
  if (count == 0)
    displayWarning(2);
  return;
}

int find_bl(char *st, int bl)
{
  textsettingstype t;
  gettextsettings(&t);
  FINDatrf fa;
  fa.natr = 2;
  for (int ia = 0; ia < ATRsize; ia++)
    if (!strncmp(ATRN[ia].atr, "ST", 2))
      break;
  if (ia == ATRsize)
    return -1;
  fa.atr[0].natr = ia;
  if (!st)
  {
    KLSt mn(ia);
    mn.draw();
    int key = mn.menu_key2();
    if (key == -1)
      return -1;
    fa.atr[0].znk = key;
  }
  else
  {
    char sstt[80];
    int ii;
    strcpy(sstt, st);
    for (ii = 0; ii < strlen(sstt); ii++)
      if (sstt[ii] == '_')
        sstt[ii] = ' ';
    unsigned *di = ATCDP + ATRN[ia].start;
    char *atcd = ATCD + ATRN[ia].end;
    for (ii = 0; ii < ATRN[ia].ncod; ii++, di++)
    {
      if (!stricmp(sstt, atcd + *di))
        break;
    }
    if (ii == ATRN[ia].ncod)
    {
      displayWarning(2);
      return -1;
    }
    fa.atr[0].znk = ii;
  }
  for (ia = 0; ia < ATRsize; ia++)
    if (!strncmp(ATRN[ia].atr, "BN", 2))
      break;
  if (ia == ATRsize)
    return -1;
  fa.atr[1].natr = ia;
  if (!bl)
  {
    GRwnd wnd2(190, 100, 430, 160, "Дом N", LIGHTBLUE, 1, 1);
    wnd2.text("Введите номер дома");
    float bln = 0;
    wnd2.readfloat(&bln, 0, 900, 3);
    wnd2.restore();
    if (!bln)
      return -1;
    fa.atr[1].znn = bln;
  }
  else
    fa.atr[1].znn = bl;
  fa.blok = model.fm.sbh;
  fa.smes = model.fm.ssh;
  unsigned ret, count = 0;
  do
  {
    ret = findobj(model, fa);
    if (ret == 0xffff)
      break;
    F_SPR spr;
    if (model.readspr(&spr, ret) != 1)
      break;
    if (strncmp(flibrt[spr.numincl].clcod, "44", 2))
      continue;
    count++;
    int dx = xwinmap(30) - xwinmap(0);
    if ((!obj_enabled(&spr)) || (spr.xmin < model.xminw) ||
        (spr.ymin < model.yminw) || (spr.xmax > (model.xmaxw - dx)) ||
        (spr.ymax > model.ymaxw))
    {
      settextjustify(t.horiz, t.vert);
      redraw(XDISRAZ(spr.ymin), YDISRAZ(spr.xmin), XDISRAZ(spr.ymax),
             YDISRAZ(spr.xmax), 64);
      setviewport(XMINE, YMINE, XMAXE, YMAXE, 1);
    }
    draw_one_obj(ret, 12);
    settwink(12, WHITE);
    {
      int at;
      char str[30];
      TEXT_PACK_MENUS tp(450, 185, 632, 7, "Здание", "", 1, 4000, 200, 1, 1);
      if ((tp.TEXT_PACK::text == NULL) || (tp.sm == NULL))
      {
        displayError(0);
        return -1;
      }
      at = ATR(&tp, ret);
      tp.draw();
      tp.textxy(1, 9, "Enter-еще,Esc-выход");
      at = tp.menu_key2();
      tp.restore();
      nottwink();
      draw_one_obj(ret, -1);
      setviewport(0, 0, XMAXMAPE, getmaxy(), 1);
      tp.del();
      ret = at;
    }
  } while (ret < 0xff00);
  if (count == 0)
  {
    displayWarning(2);
    return -1;
  }
  return 0;
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
        if (!(strncmp(flibrt[spr.numincl].clcod, "45", 2)) &&
            !(strncmp(ATRN[ma.natr].atr, "ST", 2)))
          strcpy(str, "Название:");
        else
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

void whatisit(int m, int s)
{
  char str[80];
  int key, at;
  int x, y;
  int ret = poisk(s, &x, &y);
  if (ret == -1)
    return;
  {
    TEXT_PACK_MENUS tp(450, 185, 632, 7, "Что это?", "", 1, 4000, 200, 1, 1);
    if ((tp.TEXT_PACK::text == NULL) || (tp.sm == NULL))
    {
      displayError(0);
      goto ex;
    }
    at = ATR(&tp, ret);
    tp.draw();
    tp.textxy(1, 9, "Esc,Enter - выход");
    key = tp.menu_key2();
    tp.restore();
    tp.del();
  }
ex:
  nottwink();
  draw_one_obj(ret, -1);
  setviewport(0, 0, XMAXMAPE, getmaxy(), 1);
}
