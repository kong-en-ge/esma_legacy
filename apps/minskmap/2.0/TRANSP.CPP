#include <stdio.h>
#include <string.h>
#include <math.h>

#include "coretype.hpp"
#include "utils.hpp"
#include "funcs.hpp"
#include "transp.hpp"
#include "findobj.hpp"
#include "apputils.hpp"
#include "globals.hpp"
#include "fatr.hpp"
#include "inmapapp.hpp"

CPointS transportFrom, transportTo;
int kolmet, koluz, comuz, prmt, uzl;
char stt[40], nmot1[90], ost[40];
METRO *metro;
UZEL *uzel;
UZLY *uzly;

PERES *ps;
TWOPERES *pstwo;

PERESU *psu;
OTK_KUD otk_kud[21], o_k[21];

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
      if (checkAttributeDef(maot.natr, "09", 'T'))
        sprintf(nstot, "%s", maot.zns);
      if (checkAttributeDef(maot.natr, "M1", 'T'))
        sprintf(nmu1, "%s", maot.zns);
      if (checkAttributeDef(maot.natr, "M0", 'T'))
        sprintf(nmu0, "%s", maot.zns);
      if (checkAttributeDef(maot.natr, "M2", 'T'))
        sprintf(nmu2, "%s", maot.zns);
      if (checkAttributeDef(maot.natr, "M3", 'T'))
        sprintf(nmu3, "%s", maot.zns);
    } while (!r);

    if (strlen(nstot) || strlen(nmu0))
    { //
      float dou = sqrt(pow((uzly[i].x - transportFrom.x), 2) + pow((uzly[i].y - transportFrom.y), 2));
      float dku = sqrt(pow((uzly[i].x - transportTo.x), 2) + pow((uzly[i].y - transportTo.y), 2));
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

int find_nearest_stops(CStopArray const& stops, OBSTOP* dst)
{
  int num, ob = 0;

  for (int k = 1; k <= 4; k++)
  { // for
    int i = 0;
    while (i < stops.getCount() && stops[i].vt != k)
      i++;
    if (i >= stops.getCount())
      num = -1;
    else
    {
      float dd = stops[i].d;
      num = i;
      while (i < stops.getCount())
      {
        if (stops[i].vt == k)
          if (stops[i].d < dd)
          {
            dd = stops[i].d;
            num = i;
          }
        i++;
      }
    }
    if (num != -1)
      dst[ob++] = stops[num];
  } // for

  return ob;
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
                if (checkAttributeDef(ma.natr, "NM", 'T'))
                  sprintf(mr, " %s", ma.zns);
              }
              if (checkAttributeDef(ma.natr, "09", 'T'))
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
      strcpy(vidtr, "  Автобусы:");
    if (k == 3)
      strcpy(vidtr, "  Троллейбусы:");
    if (k == 4)
      strcpy(vidtr, "  Трамваи:");
  } while (k <= 4);
  return at;
} // ATRTR


/* CTransportModule */

CTransportModule::CTransportModule(void) :
  hasSourcePos(false)
{
}

void CTransportModule::findStopsAtPos(
  CViewContext const& viewContext) const
{
  CStopArray stops;
  OBSTOP obs[4];
  CPointS pos;
  int ret = find_transport_stops(viewContext, stops, pos);
  if (!ret)
    return;
  {
    int ob = find_nearest_stops(stops, obs);
    TEXT_PACK_MENUS tp(450, 185, 632, 11, "Транспорт", "", 1, 4000, 200, 1, 1);
    if ((tp.TEXT_PACK::text == NULL) || (tp.sm == NULL))
    {
      displayError(0);
      goto ex;
    }
    viewContext.updateViewPort();
    for (int i = 0; i < ob; i++)
      gMapDataHandler->draw_one_obj(viewContext, obs[i].nobj, 12, 1);
    settwink(12, 15);
    ATRTR(&tp, ob, obs);
    tp.draw();
    tp.textxy(1, 13, "Esc,Enter - выход");
    tp.menu_key2();
    tp.restore();
    tp.del();
    nottwink();
    for (int ii = 0; ii < ob; ii++)
      gMapDataHandler->draw_one_obj(viewContext, obs[ii].nobj, -1, 1);
    viewContext.updateViewPort();
  }
ex:
}

void CTransportModule::putSourcePos(CViewContext const& viewContext)
{
  CStopArray stops;
  sourceStops.clear();

  if (!find_transport_stops(viewContext, stops, transportFrom))
  {
    setviewport(0, 0, getmaxx(), getmaxy(), 1);
    setHasSourcePos(false);
    return;
  }

  int at = 0, key;
  while (at < stops.getCount() && stops[at].vt != 1)
    at++;
  float ds = 20000.0;
  if (at < stops.getCount())
  {
    key = at;
    while (at < stops.getCount())
    {
      if (stops[at].vt == 1)
        if (stops[at].d < ds)
        {
          ds = stops[at].d;
          key = at;
        }
      at++;
    }
    sourceStops.add(stops[key]);
  }
  at = 0;
  do
  {
    if (stops[at].vt != 1)
      sourceStops.add(stops[at]);
    at++;
  } while (at < stops.getCount());
  setHasSourcePos(true);
  setviewport(0, 0, XMAXMAPE, getmaxy(), 1);
}

void CTransportModule::putTargetPos(CViewContext const& viewContext)
{
  if (sourceStops.isEmpty())
    return;

  F_SPR spr;
  CStopArray stops;
  CStopArray dstStops;

  int key, at, ob, ob3, ob4, flag = 0;
  int j1 = 0, j2 = 0, j3 = 0, j4 = 0, j5 = 0;
  if (!find_transport_stops(viewContext, stops, transportTo))
  {
    setviewport(0, 0, XMAXMAPE, getmaxy(), 1);
    return;
  }
  int gogo = 0;
  if (length(transportFrom.x - transportTo.x, transportFrom.y - transportTo.y) <= 600)
    gogo = 1;
  if (gogo == 0)
  { // пешком не дойдешь
    at = 0;
    while (at < stops.getCount() && stops[at].vt != 1)
      at++;
    float ds = 20000.0;
    if (at < stops.getCount())
    {
      key = at;
      while (at < stops.getCount())
      {
        if (stops[at].vt == 1)
          if (stops[at].d < ds)
          {
            ds = stops[at].d;
            key = at;
          }
        at++;
      }
      dstStops.add(stops[key]);
    }
    at = 0;
    do
    {
      if (stops[at].vt != 1)
        dstStops.add(stops[at]);
      at++;
    } while (at < stops.getCount());

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
      while (ikd < dstStops.getCount() && dstStops[ikd].vt != k)
        ikd++;
      while (iot < sourceStops.getCount() && sourceStops[iot].vt != k)
        iot++;
      if (iot < sourceStops.getCount() && ikd < dstStops.getCount())
      {
        ckot = 0;
        ckkd = 0;
        while (ckot < sourceStops.getCount())
        { // wh
          mrot[0] = 0;
          nstot[0] = 0;
          if (sourceStops[ckot].vt == k)
          { // on transport
            int ret2 = model.readspr(&sprot, sourceStops[ckot].nobj);
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
                    if (checkAttributeDef(maot.natr, "NM", 'T'))
                      sprintf(mrot, "%s", maot.zns);
                  if (checkAttributeDef(maot.natr, "09", 'T'))
                    sprintf(nstot, "%s", maot.zns);
                } while (r == 0);
                if ((strlen(nstot) != 0) && (((strlen(mrot) != 0) && (k != 1)) ||
                                             ((strlen(mrot) == 0) && (k == 1))))
                { // on stop where
                  ckkd = ikd;
                  ss[0] = 0;
                  while (ckkd < dstStops.getCount())
                  { // wh kuda
                    mrkd[0] = 0;
                    nstkd[0] = 0;
                    if (dstStops[ckkd].vt == k)
                    {
                      int ret2 = model.readspr(&sprkd, dstStops[ckkd].nobj);
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
                              if (checkAttributeDef(makd.natr, "NM", 'T'))
                                sprintf(mrkd, "%s", makd.zns);
                            if (checkAttributeDef(makd.natr, "09", 'T'))
                              sprintf(nstkd, "%s", makd.zns);
                          } while (r == 0);
                        } // atribute  kd
                      }   // rd kd
                    }
                    if ((k == 1) && (strlen(nstkd) != 0))
                    {
                      strcpy(otk_kud[j1].otk, nstot);
                      strcpy(otk_kud[j1].kud, nstkd);
                      otk_kud[j1].dot = sourceStops[ckot].d;
                      otk_kud[j1].dkd = dstStops[ckkd].d;
                      otk_kud[j1].vt = k;
                      otk_kud[j1].notk = sourceStops[ckot].nobj;
                      otk_kud[j1].nkud = dstStops[ckkd].nobj;
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
                        otk_kud[j1].dot = sourceStops[ckot].d;
                        otk_kud[j1].dkd = dstStops[ckkd].d;
                        otk_kud[j1].vt = k;
                        otk_kud[j1].notk = sourceStops[ckot].nobj;
                        otk_kud[j1].nkud = dstStops[ckkd].nobj;
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
      if (sourceStops[0].vt == 1)
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
        j2 = otk_met_kuda(viewContext, ps, dstStops, sourceStops[0].nobj);
        if (j2 != 0)
          flag = 1;
        else
          delete[] ps;
        if (j2 <= 2)
          ob = j2;
        else
          ob = 3;
      } // откуда на метро
      else if (dstStops[0].vt == 1)
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
        j2 = otk_met_kuda(viewContext, ps, sourceStops, dstStops[0].nobj);
        if (j2 != 0)
        {
          if (model.readspr(&sprkd, dstStops[0].nobj) == -1)
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
              if (checkAttributeDef(makd.natr, "09", 'T'))
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
        j3 = peresad(sourceStops, psu, dstStops);
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
          j4 = twoperes(sourceStops, pstwo, dstStops);
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
          j5 = three(sourceStops, pstwo, dstStops);
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
      viewContext.updateViewPort();
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
          viewContext.updateViewPort();
        }
        if (j3 != 0)
        {
          if (j2 != 0)
            tp.add("         ИЛИ        ");
          ATRPER(&tp, ob3);
          delete[] psu;
          viewContext.updateViewPort();
        }
      } // 1 peresad
      else if (j4 != 0)
      { // 2 peresad
        ATR2PER(&tp, ob4);
        delete[] pstwo;
        viewContext.updateViewPort();
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
        viewContext.updateViewPort();
      } // 3 peresad
    }   // с п
    tp.draw();
    tp.textxy(1, 12, "Esc,Enter - выход");
    key = tp.menu_key2();
    tp.restore();
    tp.del();
    viewContext.updateViewPort();
  } // вывод
ex:
  setHasSourcePos(false);
}

void CTransportModule::setHasSourcePos(bool value)
{
  if (hasSourcePos != value)
  {
    hasSourcePos = value;
    CInfoMapApplication::getInstance()->drawIndexMap();
  }
}

int findNearestStop(CStopArray const& stops, int type)
{
  int index = -1;

  for(int i = 0; i < stops.getCount(); i++)
    if (stops[i].vt == type &&
      (index < 0 || stops[i].d < stops[index].d))
      index = i;

  return index;
}

const struct
{
  int type;
  const char* code;
} transportCodes[] =
{
  {1, "62132"},
  {2, "651"},
  {3, "652"},
  {4, "653"}
};

int code2transportType(const char* code)
{
  for (int i = 0; i < sizeof(transportCodes) / sizeof(transportCodes[0]); i++)
    if (!strStartCmp(code, transportCodes[i].code))
      return transportCodes[i].type;

  return -1;
}

class CTransportFindObjectQuery: public CCustomFindObjectQuery
{
private:
  CStopArray& stops;
protected:
  virtual int addObject(int index, double distance, FLIB const* fl, int* deltaCommon, int* deltaPoints);
public:
  CTransportFindObjectQuery(CStopArray& _stops);
};

CTransportFindObjectQuery::CTransportFindObjectQuery(CStopArray& _stops) :
  stops(_stops)
{
}

int CTransportFindObjectQuery::addObject(int index, double distance, FLIB const* fl, int* deltaCommon, int* deltaPoints)
{
  for (int i = 0; i < stops.getCount(); i++)
  {
    if (distance < stops[i].d)
      break;
  }
  if (i < MAX_TRANSPORT_STOPS)
  {
    OBSTOP stop;
    stop.d = distance;
    stop.nobj = index;
    stop.vt = code2transportType(fl->clcod);
    while (stops.getCount() >= MAX_TRANSPORT_STOPS)
      stops.removeLast();
    stops.insert(i, stop);
    if (stops.getCount() >= MAX_TRANSPORT_STOPS)
      updateThreshold(stops.getLast().d, deltaCommon, deltaPoints);
  }

  return stops.getCount();
}

int transport_filter(const char* code, char loc)
{
  return loc < 4 && code2transportType(code) > 0;
}

int find_transport_stops(CViewContext const& viewContext, CStopArray& stops, CPointS& pos)
{
  if (!fillQueryPosition(pos))
    return -1;

  CTransportFindObjectQuery query(stops);
  query.viewContext = &viewContext;
  query.ignoreScaleFilter = 1;
  query.checkEnabled = 0;
  query.filter_proc = transport_filter;
  query.threshold = DXDISEKR(1000);
  query.pos = pos;

  int findObjectResult = find_objects(query);

  if (!findObjectResult)
    displayWarning(0);

  return findObjectResult > 0 ? findObjectResult : 0;
}

int find_transport_stops_at_pos(CViewContext const& viewContext, CPointS const& pos, CStopArray& stops)
{
  CTransportFindObjectQuery query(stops);
  query.viewContext = &viewContext;
  query.ignoreScaleFilter = 1;
  query.checkEnabled = 0;
  query.filter_proc = transport_filter;
  query.threshold = DXDISEKR(500);
  query.pos = pos;

  int findObjectResult = find_objects(query);

  return (findObjectResult > 0) ? findObjectResult : 0;
}

