#include <string.h>
#include <stdio.h>
#include <math.h>

#include "globalda.hpp"
#include "flibrt.hpp"
#include "fatr.hpp"
#include "menuall2.hpp"
#include "globalda.hpp"
#include "apputils.hpp"
#include "funcs.hpp"

int peresad(PERESU *ps, OBSTOP *kuda, int retkd)
{
  OBSTOP *kudan, *peres, *otkudan;
  char ss[90], sss[90];
  int r, ckot, ckkd, ckuz, j2 = 0, xot, yot, xkd, ykd;
  mk_atrf maot, makd, mauz;
  F_SPR sprot, sprkd, spruz;
  char mrot[55], mrkd[55], mruz[55];
  char nstot[40], nstkd[40], nstuz[40];
  int ret, kl;

  peres = new OBSTOP[21];
  if (peres == NULL)
  {
    displayError(0);
    return 0;
  }
  kudan = new OBSTOP[21];
  if (kudan == NULL)
  {
    displayError(0);
    delete[] peres;
    return 0;
  }
  otkudan = new OBSTOP[21];
  if (otkudan == NULL)
  {
    displayError(0);
    delete[] peres;
    delete[] kudan;
    return 0;
  }
  int num, num1, nm1 = 0, ob = 0, nm2 = 0, obo = 0;
  ss[0] = 0;
  sss[0] = 0;
  for (int k = 1; k <= 4; k++)
  { // for
    int ii = 0, ii1 = 0;
    while ((kuda[ii].vt != k) && (ii < retkd))
      ii++;
    while ((otkuda[ii1].vt != k) && (ii1 < retot))
      ii1++;
    if (ii >= retkd)
      num = -1;
    else
    {
      float dot = kuda[ii].d;
      num = ii;
      while (ii < retkd)
      {
        if (kuda[ii].vt == k)
          if (kuda[ii].d < dot)
          {
            dot = kuda[ii].d;
            num = ii;
          }
        ii++;
      }
    }
    if (ii1 >= retot)
      num1 = -1;
    else
    {
      float dot = otkuda[ii1].d;
      num1 = ii1;
      while (ii1 < retot)
      {
        if (otkuda[ii].vt == k)
          if (otkuda[ii].d < dot)
          {
            dot = otkuda[ii1].d;
            num1 = ii1;
          }
        ii1++;
      }
    }
    if (num != -1)
    {
      kudan[nm1] = kuda[num];
      ob++;
      nm1++;
    }
    if (num1 != -1)
    {
      otkudan[nm2] = otkuda[num1];
      obo++;
      nm2++;
    }
  } // for
  int iv = 0, hh;
  nstot[0] = 0;
  if (koluz != 0)
  { // on uzel
    while (iv < koluz)
    { //
      ss[0] = 0;
      xot = yot = xkd = ykd = 0;
      hh = 0;
      for (int k = 1; k <= 4; k++)
      { // for2
        int iot;
        iot = 0;
        mruz[0] = 0;
        while ((otkudan[iot].vt != k) && (iot < obo))
          iot++;
        if (iot < obo)
        {
          ckot = iot;
          if (k == 2)
            strcpy(mruz, uzel[iv].nmu1);
          else if (k == 3)
            strcpy(mruz, uzel[iv].nmu2);
          else if (k == 4)
            strcpy(mruz, uzel[iv].nmu3);
          ckot = iot;
          char nstot1[40];
          while (ckot < obo)
          { // wh otkuda
            mrot[0] = 0;
            nstot1[0] = 0;
            if (otkudan[ckot].vt == k)
            { // on tr
              int ret2 = model.readspr(&sprot, otkudan[ckot].nobj);
              if (ret2 != 1)
              {
                ckot++;
                continue;
              }
              else
              { // rd ot
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
                  maot.har += 4;
                  do
                  {
                    r = make_atr(&maot);
                    if (k != 1)
                      if (checkAttributeDef(maot.natr, "NM", 'T'))
                        sprintf(mrot, "%s", maot.zns);
                    if (checkAttributeDef(maot.natr, "09", 'T'))
                      sprintf(nstot1, "%s", maot.zns);
                  } while (!r);
                } // atribute  ot
              }   // rd ot
              if ((strlen(mrot) != 0) && (strlen(nstot1) != 0) && (k != 1))
              { // compare
                int ik = 0, ik1, iv2, iv1, fl = 0;
                char str[5], strot[5];
                while (ik < strlen(mruz))
                {
                  ik1 = 0;
                  while ((mruz[ik] != ',') && (mruz[ik] != '.') && (ik < strlen(mruz)))
                  {
                    str[ik1] = mruz[ik];
                    ik++;
                    ik1++;
                  }
                  str[ik1] = 0;
                  if ((mruz[ik] == ',') || (mruz[ik] == '.'))
                    ik++;
                  iv2 = 0;
                  while (iv2 < strlen(mrot))
                  {
                    iv1 = 0;
                    while ((mrot[iv2] != ',') && (mrot[iv2] != '.') && (iv2 < strlen(mrot)))
                    {
                      strot[iv1] = mrot[iv2];
                      iv2++;
                      iv1++;
                    }
                    strot[iv1] = 0;
                    if ((mrot[iv2] == ',') || (mrot[iv2] == '.'))
                      iv2++;
                    if ((strcmp(str, strot)) == 0)
                    {
                      if (fl == 0)
                      {
                        if (k == 2)
                          strcat(ss, "%");
                        else if (k == 3)
                          strcat(ss, "@");
                        else if (k == 4)
                          strcat(ss, "&");
                        fl = 1;
                      }
                      else
                        strcat(ss, ",");
                      strcat(ss, str);
                      if (strlen(nstot1) != 0)
                        strcpy(nstot, nstot1);
                      if (hh == 0)
                      {
                        xot = sprot.xmin;
                        yot = sprot.ymin;
                        hh = 1;
                      }
                      else
                      {
                        xot = xot / 2 + sprot.xmin / 2;
                        yot = yot / 2 + sprot.ymin / 2;
                      }
                    }
                  }
                }
              } // compare
            }   // on tr
            ckot++;
          } // wh otkuda
        }
      } // for2
      sss[0] = 0;
      if (strlen(ss) != 0)
      { // откуда есть в узле
        hh = 0;
        nstkd[0] = 0;
        for (int k = 1; k <= 4; k++)
        { // for3
          int ikd;
          ikd = 0;
          mruz[0] = 0;
          while ((kudan[ikd].vt != k) && (ikd < ob))
            ikd++;
          if (ikd < ob)
          {
            ckkd = ikd;
            if (k == 2)
              strcpy(mruz, uzel[iv].nmu1);
            else if (k == 3)
              strcpy(mruz, uzel[iv].nmu2);
            else if (k == 4)
              strcpy(mruz, uzel[iv].nmu3);
            char nstkd1[40];
            while (ckkd < ob)
            { // whkd
              mrkd[0] = 0;
              nstkd1[0] = 0;
              if (kudan[ckkd].vt == k)
              { // on tr
                int ret2 = model.readspr(&sprkd, kudan[ckkd].nobj);
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
                    makd.har += 4;
                    do
                    {
                      r = make_atr(&makd);
                      if (k != 1)
                        if (checkAttributeDef(makd.natr, "NM", 'T'))
                          sprintf(mrkd, "%s", makd.zns);
                      if (checkAttributeDef(makd.natr, "09", 'T'))
                        sprintf(nstkd1, "%s", makd.zns);
                    } while (!r);
                  } // atribute  kd
                }   // rd kd
              }     // on tr

              if ((strlen(mrkd) != 0) && (strlen(nstkd1) != 0))
              { // compare
                int ik = 0, ik1, iv2, iv1, fl = 0;
                char str[5], strot[5];
                while (ik < strlen(mruz))
                {
                  ik1 = 0;
                  while ((mruz[ik] != ',') && (mruz[ik] != '.') && (ik < strlen(mruz)))
                  {
                    str[ik1] = mruz[ik];
                    ik++;
                    ik1++;
                  }
                  str[ik1] = 0;
                  if ((mruz[ik] == ',') || (mruz[ik] == '.'))
                    ik++;
                  iv2 = 0;
                  while (iv2 < strlen(mrkd))
                  {
                    iv1 = 0;
                    while ((mrkd[iv2] != ',') && (mrkd[iv2] != '.') && (iv2 < strlen(mrkd)))
                    {
                      strot[iv1] = mrkd[iv2];
                      iv2++;
                      iv1++;
                    }
                    strot[iv1] = 0;
                    if ((mrkd[iv2] == ',') || (mrkd[iv2] == '.'))
                      iv2++;
                    if ((strcmp(str, strot)) == 0)
                    {
                      if (fl == 0)
                      {
                        if (k == 2)
                          strcat(sss, "%");
                        else if (k == 3)
                          strcat(sss, "@");
                        else if (k == 4)
                          strcat(sss, "&");
                        fl = 1;
                      }
                      else
                        strcat(sss, ",");
                      strcat(sss, str);
                      if (strlen(nstkd1) != 0)
                        strcpy(nstkd, nstkd1);
                      if (hh == 0)
                      {
                        xkd = sprkd.xmin;
                        ykd = sprkd.ymin;
                        hh = 1;
                      }
                      else
                      {
                        xkd = xkd / 2 + sprkd.xmin / 2;
                        ykd = ykd / 2 + sprkd.ymin / 2;
                      }
                    }
                  }
                }
              } // compare
              ckkd++;
            } // whkd
          }
        } // for3
        if ((strlen(sss) != 0) && (strlen(ss) != 0))
        { // данные в структуру
          float dw = sqrt(pow((uzel[iv].x - xot), 2) + pow((uzel[iv].y - yot), 2)) + 
            sqrt(pow((uzel[iv].x - xkd), 2) + pow((uzel[iv].y - ykd), 2));
          int js = 0;
          for (js = 0; js < j2; js++)
          {
            if (dw < ps[js].d)
              break;
          }
          if (js >= 10)
          {
            iv++;
            continue;
          }
          memmove(ps + js + 1, ps + js, (j2 - js) * sizeof(PERESU));
          strcpy(ps[js].stperes, uzel[iv].suzel);
          strcpy(ps[js].kd, nstkd);
          strcpy(ps[js].ot, nstot);
          strcpy(ps[js].nmot, ss);
          strcpy(ps[js].nmkd, sss);
          ps[js].d = dw;
          if (j2 < 10)
            j2++;
        } // данные в структуру
      }   // откуда есть в узле
      iv++;
    } //
  }   // on uzel
  delete[] peres;
  delete[] kudan;
  delete[] otkudan;
  return j2;
}