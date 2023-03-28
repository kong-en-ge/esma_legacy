#include <string.h>

#include "globals.hpp"
#include "fatr.hpp"
#include "apputils.hpp"
#include "funcs.hpp"
#include "codesel.hpp"

#define MAX_CITY_SEARCH 200
#define MAX_CITYNAME_SIZE 50

struct Bpunct
{
  int nob, x, y;
};


void find_punct(CViewContext const& viewContext)
{
  char szName[MAX_CITYNAME_SIZE];
  szName[0] = 0;
  GRwnd wnnd(140, 180, 420, 240, "Нас. пункт", LIGHTBLUE, 1, 1);
  wnnd.text("");
  wnnd.text("Введите название");
  wnnd.readst(szName, MAX_CITYNAME_SIZE - 1);
  wnnd.restore();
  if (!szName[0])
    return;

  strupr(szName);
  Bpunct punct[MAX_CITY_SEARCH];
  int cpunct = 0, ret = 0, ncl;
  while (((ret = next_model(ret, 1)) > 0) && (cpunct < MAX_CITY_SEARCH))
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
        if (checkAttributeName(ma.natr, "09"))
          strcpy(namst, ma.zns);
      } while (!r);
      if (!strlen(namst))
        continue;
      strupr(namst);
      int ok = 0;
      char* pNameEnd = szName + strlen(szName) - 1;
      if (*pNameEnd  == '.')
      {
        *pNameEnd = 0;
        if (!strcmp(namst, szName))
          ok++;
        *pNameEnd = '.';
      }
      else
      {
        char *sbstr = strstr(namst, szName);
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
      if (cpunct == MAX_CITY_SEARCH)
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
    redraw(viewContext, xd, yd, xd, yd, 18);
    viewContext.updateViewPort();
    draw_one_obj(viewContext, punct[ret].nob, 12, 1);
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
    draw_one_obj(viewContext, punct[ret].nob, -1, 1);
    tp.del();
    if (at == -1)
      break;
  }
  setviewport(0, 0, XMAXMAPE, getmaxy(), 1);
}

void find_rg(CViewContext const& viewContext)
{
//  textsettingstype t;
//  gettextsettings(&t);
  FINDatrf fa;
  fa.natr = 1;
  int ia = getAttributeIndex("RN");
  if (ia < 0)
    return;
  fa.atr[0].natr = ia;
  {
    KLSt mn(ia, "Выбор района");
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
    redraw(viewContext,
           XDISRAZ(spr.ymin), YDISRAZ(spr.xmin), XDISRAZ(spr.ymax),
           YDISRAZ(spr.xmax), 120);
    viewContext.updateViewPort();
    draw_one_obj(viewContext, ret, 12, 1);
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
      draw_one_obj(viewContext, ret, -1, 1);
      setviewport(0, 0, XMAXMAPE, getmaxy(), 1);
      tp.del();
      ret = at;
    }
  } while (ret < 0xff00);

  if (count == 0)
    displayWarning(2);

  return;
}
