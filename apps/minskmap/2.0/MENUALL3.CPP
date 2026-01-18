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
#include "globals.hpp"
#include "flibrt.hpp"
#include "textfile.hpp"
#include "txtpacks.hpp"
#include "mouse.hpp"
#include "drawpl.hpp"
#include "fatr.hpp"
#include "menuall2.hpp"
#include "apputils.hpp"
#include "funcs.hpp"
#include "codesel.hpp"
#include "graphapp.hpp"
#include "utils.hpp"
#include "transp.hpp"
#include "inmapapp.hpp"
#include "files.hpp"

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
    if (strStartCmp(flibrt[nkl].clcod, "62132") != 0)
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
        if (checkAttributeDef(maot.natr, "09", 'T'))
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

int comuzel()
{ // comuzel
  F_SPR spr;
  int ret = 0;
  int klm = 0;
  while ((ret = next_model(ret, 1)) > 0)
  { // who
    int nkl = models[ret - 1].numkl;
    if (strStartCmp(flibrt[nkl].clcod, "655") != 0)
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
