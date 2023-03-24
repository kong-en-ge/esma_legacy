#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <dir.h>

#include "mmfuncs.hpp"
#include "globals.hpp"
#include "flibrt.hpp"
#include "txtpacks.hpp"
#include "drawpl.hpp"
#include "fatr.hpp"
#include "menuall2.hpp"
#include "apputils.hpp"
#include "funcs.hpp"
#include "codesel.hpp"

FLIB *findobj(SPR *spr);

void find_rg(void)
{
  FINDatrf fa;
  fa.natr = 1;
  int ia = getAttributeIndex("09");
  if (ia < 0)
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
    setviewport(boundsE.left, boundsE.top, boundsE.right, boundsE.bottom, 1);
    draw_one_obj(ret, 12, 0);
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
      draw_one_obj(ret, -1, 0);
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
  int ia = getAttributeIndex("ST");
  if (ia < 0)
    return;
  fa.atr[0].natr = ia;
  {
    KLSt mn(ia, "Выбор улицы");
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
    setviewport(boundsE.left, boundsE.top, boundsE.right, boundsE.bottom, 1);
    draw_one_obj(ret, 12, 0);
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
      draw_one_obj(ret, -1, 0);
      setviewport(0, 0, XMAXMAPE, getmaxy(), 1);
      tp.del();
      ret = at;
    }
  } while (ret < 0xff00);
  if (count == 0)
    displayWarning(2);
  return;
}

int find_bl(char const *st, int bl)
{
  textsettingstype t;
  gettextsettings(&t);
  FINDatrf fa;
  fa.natr = 2;
  int ia = getAttributeIndex("ST");
  if (ia < 0)
    return -1;
  fa.atr[0].natr = ia;
  if (!st)
  {
    KLSt mn(ia, "Выбор улицы");
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
  ia = getAttributeIndex("BN");
  if (ia < 0)
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
      setviewport(boundsE.left, boundsE.top, boundsE.right, boundsE.bottom, 1);
    }
    draw_one_obj(ret, 12, 0);
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
      draw_one_obj(ret, -1, 0);
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
