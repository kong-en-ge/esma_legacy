#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
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
int ATRTR(TEXT_PACK_MENUS *tp, int obj, OBSTOP *obst);
int ATRFR(TEXT_PACK_MENUS *tp, int j1);
void ATRMT(TEXT_PACK_MENUS *tp, int ob);
void ATRTM(TEXT_PACK_MENUS *tp, int ob);
void ATRPER(TEXT_PACK_MENUS *tp, int ob);
void ATR2PER(TEXT_PACK_MENUS *tp, int ob);
int otk_met_kuda(PERES *pss, OBSTOP *otkd, int retkdot, int nn);
int peresad(PERESU *ps, OBSTOP *kuda, int retkd);
int twoperes(TWOPERES *ps, OBSTOP *kuda, int retkd);
int three(TWOPERES *ps, OBSTOP *kuda, int retkd);
int stmetro();
int stuzel(int kh);
int comuzel();
int obj_enabled(F_SPR *spr);
PERES *ps;
TWOPERES *pstwo;

struct OTK_KUD
{
  char otk[40];
  char kud[40];
  char nm[55];
  char vt;
  float dot, dkd;
  int notk;
  int nkud;
};

PERESU *psu;
OTK_KUD otk_kud[21], o_k[21];

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

int ATRTR(TEXT_PACK_MENUS *tp, int obj, OBSTOP *obst)
{ // ATRTR
  int at = 0, r;
  mk_atrf ma;
  F_SPR spr;
  char vidtr[15];
  strcpy(vidtr, "  Метро:");
  int k = 1, ck, fl = 0;
  char mr[55], nst[40];
  do
  {
    ck = 0;
    fl = 0;
    while (ck < obj)
    { // wh
      if (obst[ck].vt == k)
      {
        int ret2 = model.readspr(&spr, obst[ck].nobj);
        if (ret2 != 1)
        {
          ck++;
          continue;
        }
        else
        { // rd
          if (spr.lnh == 0)
          {
            ck++;
            continue;
          }
          else if (model.readhars((int **)&ma.har, &spr) == -1)
          {
            ck++;
            continue;
          }
          else
          { // atribute
            at = 1;
            ma.har += 4;
            mr[0] = 0;
            nst[0] = 0;
            do
            {
              r = make_atr(&ma);
              if (k != 1)
              {
                if (!(strncmp(ATRN[ma.natr].atr, "NM", 2)) &&
                    (ATRN[ma.natr].type == 'T'))
                  sprintf(mr, " %s", ma.zns);
              }
              if (!(strncmp(ATRN[ma.natr].atr, "09", 2)) &&
                  (ATRN[ma.natr].type == 'T'))
                if (k != 1)
                  sprintf(nst, "Ост. %s", ma.zns);
                else
                  sprintf(nst, "Станция %s", ma.zns);
            } while (r == 0);
            if (((fl == 0) && (strlen(mr) != 0) && (k != 1)) ||
                ((fl == 0) && (strlen(nst) != 0) && (k == 1)))
            {
              tp->add(vidtr);
              fl = 1;
            }
            if (strlen(mr) != 0)
              tp->add(mr);
            if (strlen(nst) != 0)
              tp->add(nst);
          } // atribute
        }   // rd
      }
      ck++;
    } // wh
    k++;
    if (k == 2)
      strcpy(vidtr, "  Автобусы");
    if (k == 3)
      strcpy(vidtr, "  Троллейбусы:");
    if (k == 4)
      strcpy(vidtr, "  Трамваи:");
  } while (k <= 4);
  return at;
} // ATRTR

void ATRMT(TEXT_PACK_MENUS *tp, int ob)
{ // ATRMT
  char ss[80], ss1[80];
  sprintf(ss, "На метро до ст. %s", ps[0].stperes);
  tp->add(ss);
  strcpy(ss, "Далее: ");
  tp->add(ss);
  int k = 0;
  do
  {
    if (ps[k].vt == 2)
      strcpy(ss1, "Автобусом N ");
    if (ps[k].vt == 3)
      strcpy(ss1, "Троллейбусом N ");
    if (ps[k].vt == 4)
      strcpy(ss1, "Трамваем N ");
    strcat(ss1, ps[k].nm);
    tp->add(ss1);
    if (((strcmp(ps[k].stperes, ps[k + 1].stperes) != 0) &&
         (strcmp(ps[k].stperes, ps[k + 1].stperes) != 0)) ||
        (k == ob - 1))
    {
      sprintf(ss, "До ост. %s", ps[k].ot_kd);
      tp->add(ss);
      tp->add("         ***         ");
    }
    else
      tp->add("или");
    if ((strcmp(ps[k].stperes, ps[k + 1].stperes) != 0) && (k < ob - 1))
    {
      sprintf(ss, "На метро до ст. %s", ps[k + 1].stperes);
      tp->add(ss);
      tp->add("Далее ");
    }
    k++;
  } while (k < ob);
} // ATRMT

void ATRTM(TEXT_PACK_MENUS *tp, int ob)
{ // ATRTM
  char ss1[80];
  int k = 0;
  do
  {
    if (ps[k].vt == 2)
      strcpy(ss1, "Автобусом N ");
    if (ps[k].vt == 3)
      strcpy(ss1, "Троллейбусом N ");
    if (ps[k].vt == 4)
      strcpy(ss1, "Трамваем N ");
    strcat(ss1, ps[k].nm);
    tp->add(ss1);
    if ((strcmp(ps[k].stperes, ps[k + 1].stperes) != 0) || (k == ob - 1))
    {
      sprintf(ss1, "До ст.метро %s", ps[k].stperes);
      tp->add(ss1);
    }
    else
      tp->add("или");
    k++;
  } while (k < ob);
  tp->add("  ДАЛЕЕ  ");
  sprintf(ss1, "На метро до ст. %s", stt);
  tp->add(ss1);
  tp->add("         ***         ");
} // ATRTM

void ATRPER(TEXT_PACK_MENUS *tp, int ob)
{ // ATRPER
  char ss1[100], z1[55], z2[55], z3[55], z11[55], z22[55], z33[55];
  int ik, ik1;
  char *z;
  for (unsigned k = 0; k < ob; k++)
  { // for
    z1[0] = 0;
    z2[0] = 0;
    z3[0] = 0;
    ik = 0;
    ik1 = 0;
    strcpy(ss1, psu[k].nmot);
    while (ik < strlen(ss1))
    {
      if ((ss1[ik] == '%') || (ss1[ik] == '@') || (ss1[ik] == '&'))
      {
        ik1 = 0;
        if (ss1[ik] == '%')
          z = z1;
        else if (ss1[ik] == '@')
          z = z2;
        else
          z = z3;
        ik++;
      }
      while ((ss1[ik] != '%') && (ss1[ik] != '@') && (ss1[ik] != '&') && (ik < strlen(ss1)))
      {
        *(z + ik1) = ss1[ik];
        ik++;
        ik1++;
      }
      *(z + ik1) = 0;
    }
    z11[0] = 0;
    z22[0] = 0;
    z33[0] = 0;
    ik = 0;
    ik1 = 0;
    strcpy(ss1, psu[k].nmkd);
    while (ik < strlen(ss1))
    {
      if ((ss1[ik] == '%') || (ss1[ik] == '@') || (ss1[ik] == '&'))
      {
        ik1 = 0;
        if (ss1[ik] == '%')
          z = z11;
        else if (ss1[ik] == '@')
          z = z22;
        else
          z = z33;
        ik++;
      }
      while ((ss1[ik] != '%') && (ss1[ik] != '@') && (ss1[ik] != '&') && (ik < strlen(ss1)))
      {
        *(z + ik1) = ss1[ik];
        ik++;
        ik1++;
      }
      *(z + ik1) = 0;
    }
    sprintf(ss1, "От ост. %s", psu[k].ot);
    tp->add(ss1);
    if (z1[0] != 0)
    {
      sprintf(ss1, "Автобусами N %s", z1);
      tp->add(ss1);
    }
    if (z2[0] != 0)
    {
      sprintf(ss1, "Троллейбусами N %s", z2);
      tp->add(ss1);
    }
    if (z3[0] != 0)
    {
      sprintf(ss1, "Трамваями N %s", z3);
      tp->add(ss1);
    }
    sprintf(ss1, "До ост.%s", psu[k].stperes);
    tp->add(ss1);
    tp->add("  ДАЛЕЕ  ");
    if (z11[0] != 0)
    {
      sprintf(ss1, "Автобусами N %s", z11);
      tp->add(ss1);
    }
    if (z22[0] != 0)
    {
      sprintf(ss1, "Троллейбусами N %s", z22);
      tp->add(ss1);
    }
    if (z33[0] != 0)
    {
      sprintf(ss1, "Трамваями N %s", z33);
      tp->add(ss1);
    }
    sprintf(ss1, "До ост. %s", psu[k].kd);
    tp->add(ss1);
    tp->add("         ***        ");
  } // for
} // ATRPER

void ATR2PER(TEXT_PACK_MENUS *tp, int ob)
{ // ATR2PER
  char ss1[100], z1[55], z2[55], z3[55], z11[55], z22[55], z33[55];
  char zp1[55], zp2[55], zp3[55], zmet[2], zmet1[2];
  int ik, ik1;
  char *z;
  for (unsigned k = 0; k < ob; k++)
  { // for
    z1[0] = 0;
    z2[0] = 0;
    z3[0] = 0;
    zmet1[0] = 0;
    ik = 0;
    ik1 = 0;
    strcpy(ss1, pstwo[k].nmot);
    if (ss1[0] == '*')
    {
      strcpy(zmet1, "*");
      ik++;
    }
    while (ik < strlen(ss1))
    {
      if ((ss1[ik] == '%') || (ss1[ik] == '@') || (ss1[ik] == '&'))
      {
        ik1 = 0;
        if (ss1[ik] == '%')
          z = z1;
        else if (ss1[ik] == '@')
          z = z2;
        else
          z = z3;
        ik++;
      }
      while ((ss1[ik] != '%') && (ss1[ik] != '@') && (ss1[ik] != '&') && (ik < strlen(ss1)))
      {
        *(z + ik1) = ss1[ik];
        ik++;
        ik1++;
      }
      *(z + ik1) = 0;
    }
    zp1[0] = 0;
    zp2[0] = 0;
    zp3[0] = 0;
    zmet[0] = 0;
    ik = 0;
    ik1 = 0;
    strcpy(ss1, pstwo[k].nmuz);
    if (ss1[0] == '*')
    {
      strcpy(zmet, "*");
      ik++;
    }
    while (ik < strlen(ss1))
    {
      if ((ss1[ik] == '%') || (ss1[ik] == '@') || (ss1[ik] == '&'))
      {
        ik1 = 0;
        if (ss1[ik] == '%')
          z = zp1;
        else if (ss1[ik] == '@')
          z = zp2;
        else
          z = zp3;
        ik++;
      }
      while ((ss1[ik] != '%') && (ss1[ik] != '@') && (ss1[ik] != '&') && (ik < strlen(ss1)))
      {
        *(z + ik1) = ss1[ik];
        ik++;
        ik1++;
      }
      *(z + ik1) = 0;
    }
    z11[0] = 0;
    z22[0] = 0;
    z33[0] = 0;
    ik = 0;
    ik1 = 0;
    strcpy(ss1, pstwo[k].nmkd);
    while (ik < strlen(ss1))
    {
      if ((ss1[ik] == '%') || (ss1[ik] == '@') || (ss1[ik] == '&'))
      {
        ik1 = 0;
        if (ss1[ik] == '%')
          z = z11;
        else if (ss1[ik] == '@')
          z = z22;
        else
          z = z33;
        ik++;
      }
      while ((ss1[ik] != '%') && (ss1[ik] != '@') && (ss1[ik] != '&') && (ik < strlen(ss1)))
      {
        *(z + ik1) = ss1[ik];
        ik++;
        ik1++;
      }
      *(z + ik1) = 0;
    }
    sprintf(ss1, "От ост. %s", pstwo[k].ot);
    tp->add(ss1);
    if (zmet1[0] != 0)
    {
      strcpy(ss1, "На метро ");
      tp->add(ss1);
      if ((z1[0] != 0) || (z2[0] != 0) || (z3[0] != 0))
        tp->add("или");
    }
    if (z1[0] != 0)
    {
      sprintf(ss1, "Автобусами N %s", z1);
      tp->add(ss1);
    }
    if (z2[0] != 0)
    {
      sprintf(ss1, "Троллейбусами N %s", z2);
      tp->add(ss1);
    }
    if (z3[0] != 0)
    {
      sprintf(ss1, "Трамваями N %s", z3);
      tp->add(ss1);
    }
    sprintf(ss1, "До ост.%s", pstwo[k].stperes1);
    tp->add(ss1);
    tp->add("  ЗАТЕМ  ");
    if (zmet[0] != 0)
    {
      strcpy(ss1, "На метро ");
      tp->add(ss1);
      if ((zp1[0] != 0) || (zp2[0] != 0) || (zp3[0] != 0))
        tp->add("или");
    }
    if (zp1[0] != 0)
    {
      sprintf(ss1, "Автобусами N %s", zp1);
      tp->add(ss1);
    }
    if (zp2[0] != 0)
    {
      sprintf(ss1, "Троллейбусами N %s", zp2);
      tp->add(ss1);
    }
    if (zp3[0] != 0)
    {
      sprintf(ss1, "Трамваями N %s", zp3);
      tp->add(ss1);
    }
    sprintf(ss1, "До ост. %s", pstwo[k].stperes2);
    tp->add(ss1);
    tp->add("  И НАКОНЕЦ  ");
    if (z11[0] != 0)
    {
      sprintf(ss1, "Автобусами N %s", z11);
      tp->add(ss1);
    }
    if (z22[0] != 0)
    {
      sprintf(ss1, "Троллейбусами N %s", z22);
      tp->add(ss1);
    }
    if (z33[0] != 0)
    {
      sprintf(ss1, "Трамваями N %s", z33);
      tp->add(ss1);
    }
    sprintf(ss1, "До ост. %s", pstwo[k].kd);
    tp->add(ss1);
    tp->add("         ***        ");
  } // for
} // ATR2PER

int ATRFR(TEXT_PACK_MENUS *tp, int j1)
{ // ATRFR
  int num, nm1 = 0, k, ob = 0;
  for (k = 1; k <= 4; k++)
  { // for
    int ii = 0;
    num = -1;
    while ((otk_kud[ii].vt != k) && (ii < j1))
      ii++;
    if (ii >= j1)
      num = -1;
    else
    {
      float dot = otk_kud[ii].dot, dkd = otk_kud[ii].dkd; // num=ii;
      while (ii < j1)
      {
        if ((otk_kud[ii].notk == otk_kud[ii].nkud) ||
            ((strcmp(otk_kud[ii].otk, otk_kud[ii].kud)) == 0))
        {
          ii++;
          continue;
        }
        if (otk_kud[ii].vt == k)
          if ((otk_kud[ii].dot <= dot) && (otk_kud[ii].dkd <= dkd))
          {
            dot = otk_kud[ii].dot;
            dkd = otk_kud[ii].dkd;
            num = ii;
          }
        ii++;
      }
    }
    if (num != -1)
    {
      memmove(&o_k[nm1], &otk_kud[num], sizeof(OTK_KUD));
      ob++;
      nm1++;
    }
  } // for
  char ss1[80], ss[80];
  if (ob != 0)
  { // yes
    k = 1;
    strcpy(ss1, "   Метро:");
    do
    {
      int ck = 0, fl = 0;
      while (ck < ob)
      { // wh
        if (o_k[ck].vt == k)
        {
          if (fl == 0)
          {
            tp->add(ss1);
            fl = 1;
          }
          if (k != 1)
            sprintf(ss, "От ост. %s", o_k[ck].otk);
          else
            sprintf(ss, "От ст. %s", o_k[ck].otk);
          tp->add(ss);
          if (k != 1)
          {
            sprintf(ss, "N %s", o_k[ck].nm);
            tp->add(ss);
          }
          if (k != 1)
            sprintf(ss, "до ост. %s", o_k[ck].kud);
          else
            sprintf(ss, "до ст. %s", o_k[ck].kud);
          tp->add(ss);
        }
        ck++;
      } // wh
      k++;
      if (k == 2)
        strcpy(ss1, "  Автобусами:");
      if (k == 3)
        strcpy(ss1, "  Троллейбусами:");
      if (k == 4)
        strcpy(ss1, "  Трамваями:");
    } while (k <= 4);
    return ob;
  } // yes
  else
  {
    tp->add("     ПЕШКОМ!   ");
    return 1;
  }
} // ATRFR

void howstop()
{
  obstop = new OBSTOP[20];
  OBSTOP *obs;
  obs = new OBSTOP[4];
  char str[80];
  int x, y, ob = 0, num, nm1 = 0;
  int ret = poisk(1, &x, &y);
  if (!ret)
  {
    delete[] obstop;
    delete[] obs;
    return;
  }
  {
    for (int k = 1; k <= 4; k++)
    { // for
      int ii = 0;
      while ((obstop[ii].vt != k) && (ii < ret))
        ii++;
      if (ii >= ret)
        num = -1;
      else
      {
        float dd = obstop[ii].d;
        num = ii;
        while (ii < ret)
        {
          if (obstop[ii].vt == k)
            if (obstop[ii].d < dd)
            {
              dd = obstop[ii].d;
              num = ii;
            }
          ii++;
        }
      }
      if (num != -1)
      {
        obs[nm1] = obstop[num];
        ob++;
        nm1++;
      }
    } // for
    TEXT_PACK_MENUS tp(450, 185, 632, 11, "Транспорт", "", 1, 4000, 200, 1, 1);
    if ((tp.TEXT_PACK::text == NULL) || (tp.sm == NULL))
    {
      displayError(0);
      delete[] obstop;
      delete[] obs;
      goto ex;
    }
    setviewport(XMINE, YMINE, XMAXE, YMAXE, 1);
    for (int i = 0; i < ob; i++)
      draw_one_obj(obs[i].nobj, 12, 1);
    settwink(12, 15);
    ATRTR(&tp, ob, obs);
    tp.draw();
    tp.textxy(1, 13, "Esc,Enter - выход");
    tp.menu_key2();
    tp.restore();
    tp.del();
    nottwink();
    for (int ii = 0; ii < ob; ii++)
      draw_one_obj(obs[ii].nobj, -1, 1);
    setviewport(0, 0, XMAXMAPE, getmaxy(), 1);
    delete[] obstop;
    delete[] obs;
  }
ex:
}

int stmetro()
{ // metro
  F_SPR spr;
  mk_atrf maot;
  int ret = 0, r;
  int klm = 0;
  char nstot[40];
  while ((ret = next_model(ret)) > 0)
  { // who
    int nkl = models[ret - 1].numkl;
    if (strncmp(flibrt[nkl].clcod, "62132", 5) != 0)
      continue;
    if (flibrt[nkl].loc != 1)
      continue;
    for (unsigned i = model.pm.start; i <= model.pm.end; i++)
    { // for1
      if (model.readspr(&spr, i) != 1)
        continue;
      if (spr.lnh == 0)
        continue;
      if (model.readhars((int **)&maot.har, &spr) == -1)
        continue;
      maot.har += 4;
      do
      {
        r = make_atr(&maot);
        if (!(strncmp(ATRN[maot.natr].atr, "09", 2)) &&
            (ATRN[maot.natr].type == 'T'))
        {
          sprintf(nstot, "%s", maot.zns);
          r = 1;
        }
      } while (r == 0);
      if (strlen(nstot) != 0)
      {
        int prx = 0;
        for (unsigned ii = 0; ii < klm; ii++)
        {
          if ((strcmp(nstot, metro[ii].smetro)) == 0)
          {
            metro[ii].x = metro[ii].x / 2 + spr.xmin / 2;
            metro[ii].y = metro[ii].y / 2 + spr.ymin / 2;
            prx = 1;
            break;
          }
        }
        if (prx == 0)
        {
          metro[klm].x = spr.xmin;
          metro[klm].y = spr.ymin;
          metro[klm].n = i;
          strcpy(metro[klm].smetro, nstot);
          if (klm < 20)
            klm++;
        }
      }
    } // for1
  }   // who
  return klm;
} // metro

int stuzel(int kh)
{ // uzel
  F_SPR spr;
  mk_atrf maot;
  int r, i;
  int klm = 0;
  float koeff;
  char nstot[40], nmu0[25], nmu1[55], nmu2[55], nmu3[55];
  for (i = 0; i < comuz; i++)
  { // for
    if (model.readspr(&spr, uzly[i].n) != 1)
      continue;
    if (spr.lnh == 0)
      continue;
    if (model.readhars((int **)&maot.har, &spr) == -1)
      continue;
    maot.har += 4;
    nstot[0] = 0;
    nmu0[0] = 0;
    nmu1[0] = 0;
    nmu2[0] = 0;
    nmu3[0] = 0;
    do
    {
      r = make_atr(&maot);
      if (!(strncmp(ATRN[maot.natr].atr, "09", 2)) &&
          (ATRN[maot.natr].type == 'T'))
        sprintf(nstot, "%s", maot.zns);
      if (!(strncmp(ATRN[maot.natr].atr, "M1", 2)) &&
          (ATRN[maot.natr].type == 'T'))
        sprintf(nmu1, "%s", maot.zns);
      if (!(strncmp(ATRN[maot.natr].atr, "M0", 2)) &&
          (ATRN[maot.natr].type == 'T'))
        sprintf(nmu0, "%s", maot.zns);
      if (!(strncmp(ATRN[maot.natr].atr, "M2", 2)) &&
          (ATRN[maot.natr].type == 'T'))
        sprintf(nmu2, "%s", maot.zns);
      if (!(strncmp(ATRN[maot.natr].atr, "M3", 2)) &&
          (ATRN[maot.natr].type == 'T'))
        sprintf(nmu3, "%s", maot.zns);
    } while (r == 0);
    if ((strlen(nstot) != 0) || (strlen(nmu0) != 0))
    { //
      float dou = sqrt(pow((uzly[i].x - xco), 2) + pow((uzly[i].y - yco), 2));
      float dku = sqrt(pow((uzly[i].x - xck), 2) + pow((uzly[i].y - yck), 2));
      float sd = dou + dku;
      int js = 0;
      for (js = 0; js < klm; js++)
      {
        if (sd < uzel[js].d)
          break;
      }
      if (js >= kh)
      {
        i++;
        continue;
      }
      memmove(uzel + js + 1, uzel + js, (klm - js) * sizeof(UZEL));
      uzel[js].x = spr.xmin;
      uzel[js].y = spr.ymin;
      uzel[js].n = uzly[i].n;
      uzel[js].d = sd;
      strcpy(uzel[js].suzel, nstot);
      strcpy(uzel[js].nmu0, nmu0);
      strcpy(uzel[js].nmu1, nmu1);
      strcpy(uzel[js].nmu2, nmu2);
      strcpy(uzel[js].nmu3, nmu3);
      if (klm < kh)
        klm++;
    } //
  }   // for
  return klm;
} // uzel

int comuzel()
{ // comuzel
  F_SPR spr;
  int ret = 0;
  int klm = 0;
  while ((ret = next_model(ret, 1)) > 0)
  { // who
    int nkl = models[ret - 1].numkl;
    if (strncmp(flibrt[nkl].clcod, "655", 3) != 0)
      continue;
    if (flibrt[nkl].loc != 1)
      continue;
    for (unsigned i = model.pm.start; i <= model.pm.end; i++)
    { // for1
      if (model.readspr(&spr, i) != 1)
        continue;
      uzly[klm].x = spr.xmin;
      uzly[klm].y = spr.ymin;
      uzly[klm].n = i;
      if (klm < 120)
        klm++;
    } // for1
  }   // who
  return klm;
} // comuzel

void where_from()
{
  obstop = new OBSTOP[21];
  retot = poisk(1, &xo, &yo);
  if (pr == 1)
    delete[] otkuda;
  if (!retot)
  {
    setviewport(0, 0, getmaxx(), getmaxy(), 1);
    if (pr == 1)
    {
      pr = 0;
      drawcram();
    }
    delete[] obstop;
    return;
  }
  xco = xo;
  yco = yo;
  otkuda = new OBSTOP[21];
  int at = 0, key, kl = 0;
  while ((obstop[at].vt != 1) && (at < retot))
    at++;
  float ds = 20000.0;
  if (at < retot)
  {
    key = at;
    while (at < retot)
    {
      if (obstop[at].vt == 1)
        if (obstop[at].d < ds)
        {
          ds = obstop[at].d;
          key = at;
        }
      at++;
    }
    memmove(&otkuda[kl], &obstop[key], sizeof(OBSTOP));
    kl++;
  }
  at = 0;
  do
  {
    if (obstop[at].vt != 1)
    {
      memmove(&otkuda[kl], &obstop[at], sizeof(OBSTOP));
      kl++;
    }
    at++;
  } while (at < retot);
  retot = kl;
  pr = 1;
  setviewport(0, 0, getmaxx(), getmaxy(), 1);
  if (!GdEGA)
    drawcram();
  setviewport(0, 0, XMAXMAPE, getmaxy(), 1);

  delete[] obstop;
}

void where()
{
  if (retot == 0)
    return;
  F_SPR spr;
  OBSTOP *kuda;
  obstop = new OBSTOP[21];
  int key, at, ob, ob3, ob4, flag = 0;
  int retkd = poisk(1, &xo, &yo);
  int j1 = 0, j2 = 0, j3 = 0, j4 = 0, j5 = 0;
  if (!retkd)
  {
    delete[] obstop;
    setviewport(0, 0, XMAXMAPE, getmaxy(), 1);
    return;
  }
  xck = xo;
  yck = yo;
  int gogo = 0;
  if (length(xco - xck, yco - yck) <= 600)
    gogo = 1;
  if (gogo == 0)
  { // пешком не дойдешь
    kuda = new OBSTOP[21];
    at = 0;
    while ((obstop[at].vt != 1) && (at < retkd))
      at++;
    float ds = 20000.0;
    int kl = 0;
    if (at < retkd)
    {
      key = at;
      while (at < retkd)
      {
        if (obstop[at].vt == 1)
          if (obstop[at].d < ds)
          {
            ds = obstop[at].d;
            key = at;
          }
        at++;
      }
      memmove(&kuda[kl], &obstop[key], sizeof(OBSTOP));
      kl++;
    }
    at = 0;
    do
    {
      if (obstop[at].vt != 1)
      {
        memmove(&kuda[kl], &obstop[at], sizeof(OBSTOP));
        kl++;
      }
      at++;
    } while (at < retkd);
    retkd = kl;
    char ss[55], ss1[55];
    int r, ckot, ckkd;
    mk_atrf maot, makd;
    F_SPR sprot, sprkd;
    char mrot[55], mrkd[55], nstot[40], nstkd[40];
    for (int k = 1; k <= 4; k++)
    { // for
      int iot, ikd;
      iot = 0;
      ikd = 0;
      while ((kuda[ikd].vt != k) && (ikd < retkd))
        ikd++;
      while ((otkuda[iot].vt != k) && (iot < retot))
        iot++;
      if ((iot < retot) && (ikd < retkd))
      {
        ckot = 0;
        ckkd = 0;
        while (ckot < retot)
        { // wh
          mrot[0] = 0;
          nstot[0] = 0;
          if (otkuda[ckot].vt == k)
          { // on transport
            int ret2 = model.readspr(&sprot, otkuda[ckot].nobj);
            if (ret2 != 1)
            {
              ckot++;
              continue;
            }
            else
            { // rdot
              if (sprot.lnh == 0)
              {
                ckot++;
                continue;
              }
              else if (model.readhars((int **)&maot.har, &sprot) == -1)
              {
                ckot++;
                continue;
              }
              else
              { // atribute ot
                at = 1;
                maot.har += 4;
                do
                {
                  r = make_atr(&maot);
                  if (k != 1)
                    if (!(strncmp(ATRN[maot.natr].atr, "NM", 2)) &&
                        (ATRN[maot.natr].type == 'T'))
                      sprintf(mrot, "%s", maot.zns);
                  if (!(strncmp(ATRN[maot.natr].atr, "09", 2)) &&
                      (ATRN[maot.natr].type == 'T'))
                    sprintf(nstot, "%s", maot.zns);
                } while (r == 0);
                if ((strlen(nstot) != 0) && (((strlen(mrot) != 0) && (k != 1)) ||
                                             ((strlen(mrot) == 0) && (k == 1))))
                { // on stop where
                  ckkd = ikd;
                  ss[0] = 0;
                  while (ckkd < retkd)
                  { // wh kuda
                    mrkd[0] = 0;
                    nstkd[0] = 0;
                    if (kuda[ckkd].vt == k)
                    {
                      int ret2 = model.readspr(&sprkd, kuda[ckkd].nobj);
                      if (ret2 != 1)
                      {
                        ckkd++;
                        continue;
                      }
                      else
                      { // rd kd
                        if (sprkd.lnh == 0)
                        {
                          ckkd++;
                          continue;
                        }
                        else if (model.readhars((int **)&makd.har, &sprkd) == -1)
                        {
                          ckkd++;
                          continue;
                        }
                        else
                        { // atribute kd
                          at = 1;
                          makd.har += 4;
                          ss[0] = 0;
                          do
                          {
                            r = make_atr(&makd);
                            if (k != 1)
                              if (!(strncmp(ATRN[makd.natr].atr, "NM", 2)) &&
                                  (ATRN[makd.natr].type == 'T'))
                                sprintf(mrkd, "%s", makd.zns);
                            if (!(strncmp(ATRN[makd.natr].atr, "09", 2)) &&
                                (ATRN[makd.natr].type == 'T'))
                              sprintf(nstkd, "%s", makd.zns);
                          } while (r == 0);
                        } // atribute  kd
                      }   // rd kd
                    }
                    if ((k == 1) && (strlen(nstkd) != 0))
                    {
                      strcpy(otk_kud[j1].otk, nstot);
                      strcpy(otk_kud[j1].kud, nstkd);
                      otk_kud[j1].dot = otkuda[ckot].d;
                      otk_kud[j1].dkd = kuda[ckkd].d;
                      otk_kud[j1].vt = k;
                      otk_kud[j1].notk = otkuda[ckot].nobj;
                      otk_kud[j1].nkud = kuda[ckkd].nobj;
                      if (j1 < 20)
                        j1++;
                    }
                    if ((strlen(mrkd) != 0) && (strlen(nstkd) != 0) && (k != 1))
                    { // compare
                      int ik = 0, ik1, iv, iv1;
                      char str[5], strkd[5];
                      while (ik < strlen(mrot))
                      {
                        ik1 = 0;
                        while ((mrot[ik] != ',') && (ik < strlen(mrot)))
                        {
                          str[ik1] = mrot[ik];
                          ik++;
                          ik1++;
                        }
                        str[ik1] = 0;
                        if (mrot[ik] == ',')
                          ik++;
                        iv = 0;
                        while (iv < strlen(mrkd))
                        {
                          iv1 = 0;
                          while ((mrkd[iv] != ',') && (iv < strlen(mrkd)))
                          {
                            strkd[iv1] = mrkd[iv];
                            iv++;
                            iv1++;
                          }
                          strkd[iv1] = 0;
                          if (mrkd[iv] == ',')
                            iv++;
                          if ((strcmp(str, strkd)) == 0)
                          {
                            if (ss[0] != 0)
                              strcat(ss, ",");
                            strcat(ss, str);
                          }
                        }
                      }
                      if ((strlen(ss) != 0) && (j1 < 20))
                      {
                        strcpy(otk_kud[j1].otk, nstot);
                        strcpy(otk_kud[j1].kud, nstkd);
                        strcpy(otk_kud[j1].nm, ss);
                        otk_kud[j1].dot = otkuda[ckot].d;
                        otk_kud[j1].dkd = kuda[ckkd].d;
                        otk_kud[j1].vt = k;
                        otk_kud[j1].notk = otkuda[ckot].nobj;
                        otk_kud[j1].nkud = kuda[ckkd].nobj;
                        if (j1 < 20)
                          j1++;
                      }
                    } // compare
                    ckkd++;
                  } // wh kuda
                }   // on stop where
              }     // atribute ot
            }       // rd  ot
          }         // on transport
          ckot++;
        } // wh
      }
    } // for
    if (j1 == 0)
    { // определить пересадку
      if (otkuda[0].vt == 1)
      { // откуда на метро
        if (prmt == 0)
        {
          if ((metro = new METRO[21]) == NULL)
          {
            displayError(0);
            goto ex;
          }
          kolmet = stmetro();
          prmt = 1;
        }
        if ((ps = new PERES[11]) == NULL)
        {
          displayError(0);
          goto ex;
        }
        j2 = otk_met_kuda(ps, kuda, retkd, otkuda[0].nobj);
        if (j2 != 0)
          flag = 1;
        else
          delete[] ps;
        if (j2 <= 2)
          ob = j2;
        else
          ob = 3;
      } // откуда на метро
      else if (kuda[0].vt == 1)
      { // куда на метро
        if (prmt == 0)
        {
          if ((metro = new METRO[21]) == NULL)
          {
            displayError(0);
            goto ex;
          }
          kolmet = stmetro();
          prmt = 1;
        }
        if ((ps = new PERES[11]) == NULL)
        {
          displayError(0);
          goto ex;
        }
        j2 = otk_met_kuda(ps, otkuda, retot, kuda[0].nobj);
        if (j2 != 0)
        {
          if (model.readspr(&sprkd, kuda[0].nobj) == -1)
          {
            displayError(1);
            goto ex;
          }
          if (model.readhars((int **)&makd.har, &sprkd) == -1)
          {
            displayError(1);
            goto ex;
          }
          else
          { // atribute
            at = 1;
            makd.har += 4;
            stt[0] = 0;
            do
            {
              r = make_atr(&makd);
              if (!(strncmp(ATRN[makd.natr].atr, "09", 2)) &&
                  (ATRN[makd.natr].type == 'T'))
                sprintf(stt, "%s", makd.zns);
            } while (r == 0);
          } // atribute
          flag = 2;
        }
        else
          delete[] ps;
        if (j2 <= 2)
          ob = j2;
        else
          ob = 3;
      } // куда на метро
      if (uzl == 0)
      {
        if ((uzly = new UZLY[120]) == NULL)
        {
          displayError(0);
          goto ex;
        }
        uzl = 1;
        comuz = comuzel();
      }
      if ((uzel = new UZEL[41]) == NULL)
      {
        displayError(0);
        goto ex;
      }
      koluz = stuzel(40);
      j3 = 0;
      if (koluz != 0)
      {
        if ((psu = new PERESU[11]) == NULL)
        {
          displayError(0);
          goto ex;
        }
        j3 = peresad(psu, kuda, retkd);
      }
      delete[] uzel;
      if (j3 == 0)
        delete[] psu;
      if (j3 <= 2)
        ob3 = j3;
      else
        ob3 = 3;
      if ((j2 == 0) && (j3 == 0))
      { // две пересадки
        if ((uzel = new UZEL[51]) == NULL)
        {
          displayError(0);
          goto ex;
        }
        koluz = stuzel(50);
        j4 = 0;
        if (koluz != 0)
        {
          if ((pstwo = new TWOPERES[11]) == NULL)
          {
            displayError(0);
            goto ex;
          }
          j4 = twoperes(pstwo, kuda, retkd);
        }
        if (j4 != 0)
        {
          if (j4 <= 2)
            ob4 = j4;
          else
            ob4 = 3;
        }
        if (j4 == 0)
        {
          j5 = 0;
          j5 = three(pstwo, kuda, retkd);
          if (j5 == 0)
          {
            displayWarning(4);
            delete[] pstwo;
          }
          else
            ob4 = 1;
        }
        delete[] uzel;
      } // две пересадки
    }   // определить пересадку
  }     // пешком не дойдешь
  if ((j1 != 0) || (j2 != 0) || (j3 != 0) || (j4 != 0) || (j5 != 0) || (gogo == 1))
  { // вывод
    TEXT_PACK_MENUS tp(400, 185, 632, 10, "Как доехать", "", 1, 4000, 200, 1, 1);
    if ((tp.TEXT_PACK::text == NULL) || (tp.sm == NULL))
    {
      displayError(0);
      goto ex;
    }
    if (gogo == 1)
      tp.add("    ПЕШКОМ!   ");
    else if (j1 != 0)
    { // пр
      ob = ATRFR(&tp, j1);
      setviewport(XMINE, YMINE, XMAXE, YMAXE, 1);
    } // пр
    else if ((j2 != 0) || (j3 != 0) || (j4 != 0) || (j5 != 0))
    { // с п
      if ((j2 != 0) || (j3 != 0))
      { // 1 peresad
        if (j2 != 0)
        {
          if (flag == 1)
            ATRMT(&tp, ob);
          else if (flag == 2)
            ATRTM(&tp, ob);
          delete[] ps;
          setviewport(XMINE, YMINE, XMAXE, YMAXE, 1);
        }
        if (j3 != 0)
        {
          if (j2 != 0)
            tp.add("         ИЛИ        ");
          ATRPER(&tp, ob3);
          delete[] psu;
          setviewport(XMINE, YMINE, XMAXE, YMAXE, 1);
        }
      } // 1 peresad
      else if (j4 != 0)
      { // 2 peresad
        ATR2PER(&tp, ob4);
        delete[] pstwo;
        setviewport(XMINE, YMINE, XMAXE, YMAXE, 1);
      } // 2 peresad
      else if (j5 != 0)
      { // 3 peresad
        char *z, z1[55], z2[55], z3[55], ssa[90];
        int ik, ik1;
        z1[0] = 0;
        z2[0] = 0;
        z3[0] = 0;
        ik = 0;
        ik1 = 0;
        strcpy(ssa, nmot1);
        while (ik < strlen(ssa))
        {
          if ((ssa[ik] == '%') || (ssa[ik] == '@') || (ssa[ik] == '&'))
          {
            ik1 = 0;
            if (ssa[ik] == '%')
              z = z1;
            else if (ssa[ik] == '@')
              z = z2;
            else
              z = z3;
            ik++;
          }
          while ((ssa[ik] != '%') && (ssa[ik] != '@') && (ssa[ik] != '&') && (ik < strlen(ssa)))
          {
            *(z + ik1) = ssa[ik];
            ik++;
            ik1++;
          }
          *(z + ik1) = 0;
        }
        sprintf(ssa, "От ост. %s", ost);
        tp.add(ssa);
        if (z1[0] != 0)
        {
          sprintf(ssa, "Автобусами N %s", z1);
          tp.add(ssa);
        }
        if (z2[0] != 0)
        {
          sprintf(ssa, "Троллейбусами N %s", z2);
          tp.add(ssa);
        }
        if (z3[0] != 0)
        {
          sprintf(ssa, "Трамваями N %s", z3);
          tp.add(ssa);
        }
        sprintf(ssa, "До ост.%s", pstwo[0].ot);
        tp.add(ssa);
        tp.add("  ДАЛЕЕ  ");
        ATR2PER(&tp, ob4);
        delete[] pstwo;
        setviewport(XMINE, YMINE, XMAXE, YMAXE, 1);
      } // 3 peresad
    }   // с п
    tp.draw();
    tp.textxy(1, 12, "Esc,Enter - выход");
    key = tp.menu_key2();
    tp.restore();
    tp.del();
    setviewport(XMINE, YMINE, XMAXE, YMAXE, 1);
  } // вывод
ex:
  pr = 0;
  setviewport(0, 0, getmaxx(), getmaxy(), 1);
  if (!GdEGA)
    drawcram();
  setviewport(0, 0, XMAXMAPE, getmaxy(), 1);
  delete[] kuda;
  delete[] otkuda;
  delete[] obstop;
}
