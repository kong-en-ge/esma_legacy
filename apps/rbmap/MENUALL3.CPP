#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <io.h>
#include <fcntl.h>
#include <dir.h>

#include "dbf.hpp"
#include "geomodel.hpp"
#include "bigpath.hpp"
#include "tpedit.hpp"
#include "button.hpp"
#include "globalda.hpp"
#include "flibrt.hpp"
#include "textfile.hpp"
#include "txtpacks.hpp"
#include "mouse.hpp"
#include "drawpl.hpp"
#include "fatr.hpp"
#include "menuall2.hpp"
#include "globalda.hpp"
#include "apputils.hpp"
#include "funcs.hpp"

FLIB *findobj(SPR *spr);
int findobjn(SPR *spr);
int obj_enabled(F_SPR *spr);
int next_model(int startpos, int nc = 0);

struct KLSt : public Big_Menu
{
  int nchar, numat;
  char cod[21];
  void draw_one(int num);
  int key_handler(int key);
  int numinkl(char *cod, int sim);
  KLSt(int ia);
};

KLSt::KLSt(int ia) : Big_Menu(150, 100, 470, 10, "Выбор района",
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

struct Bpunct
{
  int nob, x, y;
};

void find_punct(void)
{
  char fist[50];
  fist[0] = 0;
  GRwnd wnnd(140, 180, 420, 240, "Нас. пункт", LIGHTBLUE, 1, 1);
  wnnd.text("");
  wnnd.text("Введите название");
  wnnd.readst(fist, 49);
  wnnd.restore();
  if (!fist[0])
    return;
  strupr(fist);
  Bpunct punct[200];
  int cpunct = 0, ret = 0, ncl;
  while (((ret = next_model(ret, 1)) > 0) && (cpunct < 200))
  {
    ncl = models[ret - 1].numkl;
    if (flibrt[ncl].clcod[0] != '4')
      continue;
    for (unsigned i = model.pm.start; i <= model.pm.end; i++)
    {
      F_SPR spr;
      if (model.readspr(&spr, i) != 1)
        continue;
      mk_atrf ma;
      if ((spr.sbh == 0) || (model.readhars((int **)&ma.har, &spr) == -1))
        continue;
      ma.har += 4;
      int r;
      char namst[100];
      namst[0] = 0;
      do
      {
        r = make_atr(&ma);
        if (!strncmp(ATRN[ma.natr].atr, "09", 2))
          strcpy(namst, ma.zns);
      } while (r == 0);
      if (!strlen(namst))
        continue;
      strupr(namst);
      int ok = 0;
      if (fist[strlen(fist) - 1] == '.')
      {
        fist[strlen(fist) - 1] = 0;
        if (!strcmp(namst, fist))
          ok++;
        fist[strlen(fist)] = '.';
      }
      else
      {
        char *sbstr = strstr(namst, fist);
        if ((sbstr) && ((sbstr == namst) || (*(sbstr - 1) == ' ') || (*(sbstr - 1) == '.')))
          ok++;
      }
      if (ok)
      {
        punct[cpunct].nob = i;
        punct[cpunct].x = spr.xmin / 2 + spr.xmax / 2;
        punct[cpunct].y = spr.ymin / 2 + spr.ymax / 2;
        cpunct++;
      }
      if (cpunct == 200)
        break;
    }
  }
  if (!cpunct)
  {
    displayWarning(2);
    return;
  }
  for (ret = 0; ret < cpunct; ret++)
  {
    double xd = XDISRAZ(punct[ret].y);
    double yd = YDISRAZ(punct[ret].x);
    redraw(xd, yd, xd, yd, 18);
    setviewport(XMINE, YMINE, XMAXE, YMAXE, 1);
    draw_one_obj(punct[ret].nob, 12, 1);
    settwink(12, 10);
    int at;
    TEXT_PACK_MENUS tp(450, 185, 632, 7, "Нас.пункт", "", 1, 4000, 200, 1, 1);
    if ((tp.TEXT_PACK::text == NULL) || (tp.sm == NULL))
    {
      displayError(0);
      return;
    }
    at = ATR(&tp, punct[ret].nob);
    tp.draw();
    tp.textxy(1, 9, "Enter-еще,Esc-выход");
    at = tp.menu_key2();
    tp.restore();
    nottwink();
    draw_one_obj(punct[ret].nob, -1, 1);
    tp.del();
    if (at == -1)
      break;
  }
  setviewport(0, 0, XMAXMAPE, getmaxy(), 1);
}

void find_rg(void)
{
  textsettingstype t;
  gettextsettings(&t);
  FINDatrf fa;
  fa.natr = 1;
  for (int ia = 0; ia < ATRsize; ia++)
    if (!strncmp(ATRN[ia].atr, "RN", 2))
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
    if (strncmp(flibrt[spr.numincl].clcod, "8115", 4))
      continue;
    count++;
    redraw(XDISRAZ(spr.ymin), YDISRAZ(spr.xmin), XDISRAZ(spr.ymax),
           YDISRAZ(spr.xmax), 120);
    setviewport(XMINE, YMINE, XMAXE, YMAXE, 1);
    draw_one_obj(ret, 12, 1);
    settwink(12, 4);
    {
      int at;
      char str[30];
      TEXT_PACK_MENUS tp(450, 185, 632, 7, "Район", "", 1, 4000, 200, 1, 1);
      if ((tp.TEXT_PACK::text == NULL) || (tp.sm == NULL))
      {
        displayError(0);
        return;
      }
      at = ATR(&tp, ret);
      tp.draw();
      tp.textxy(1, 9, "Enter,Esc-выход");
      at = tp.menu_key2();
      tp.restore();
      nottwink();
      draw_one_obj(ret, -1, 1);
      setviewport(0, 0, XMAXMAPE, getmaxy(), 1);
      tp.del();
      ret = at;
    }
  } while (ret < 0xff00);
  if (count == 0)
    displayWarning(2);
  return;
}
