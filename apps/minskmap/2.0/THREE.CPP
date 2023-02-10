#include <string.h>
#include <values.h>
#include <stdio.h>
#include <math.h>

#include "globalda.hpp"
#include "flibrt.hpp"
#include "fatr.hpp"
#include "menuall2.hpp"
#include "globalda.hpp"
#include "apputils.hpp"
#include "funcs.hpp"

int three(TWOPERES *ps, OBSTOP *kuda, int retkd)
{
  OBSTOP *kudan, *peres, *otkudan;
  char ss[90], sss[90], ssss[90], sssss[90];
  int r, ckot, ckkd, ckuz, j2 = 0, xot, yot, xkd, ykd;
  mk_atrf maot, makd, mauz;
  F_SPR sprot, sprkd, spruz;
  char mrot[55], mrkd[55], mruz1[55], mruz2[55], mruz3[55];
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
  ssss[0] = 0;
  sssss[0] = 0;
  ps[0].d = MAXFLOAT;
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
  int iv = 0, hh, ivk, ivs, iv3;
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
        mruz1[0] = 0;
        while ((otkudan[iot].vt != k) && (iot < obo))
          iot++;
        if (iot < obo)
        {
          ckot = iot;
          if (k == 1)
            strcpy(mruz1, uzel[iv].nmu0);
          else if (k == 2)
            strcpy(mruz1, uzel[iv].nmu1);
          else if (k == 3)
            strcpy(mruz1, uzel[iv].nmu2);
          else if (k == 4)
            strcpy(mruz1, uzel[iv].nmu3);
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
                while (ik < strlen(mruz1))
                {
                  ik1 = 0;
                  while ((mruz1[ik] != ',') && (mruz1[ik] != '.') && (ik < strlen(mruz1)))
                  {
                    str[ik1] = mruz1[ik];
                    ik++;
                    ik1++;
                  }
                  str[ik1] = 0;
                  if ((mruz1[ik] == ',') || (mruz1[ik] == '.'))
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

      if (strlen(ss) != 0)
      { // откуда есть в узле
        hh = 0;
        nstkd[0] = 0;
        int ivk = 0;
        while (ivk < koluz)
        { // two
          sss[0] = 0;
          for (int k = 1; k <= 4; k++)
          { // for3
            int ikd;
            ikd = 0;
            mruz2[0] = 0;
            while ((kudan[ikd].vt != k) && (ikd < ob))
              ikd++;
            if (ikd < ob)
            {
              ckkd = ikd;
              if (k == 1)
                strcpy(mruz2, uzel[ivk].nmu0);
              else if (k == 2)
                strcpy(mruz2, uzel[ivk].nmu1);
              else if (k == 3)
                strcpy(mruz2, uzel[ivk].nmu2);
              else if (k == 4)
                strcpy(mruz2, uzel[ivk].nmu3);
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
                      } while (r == 0);
                    } // atribute  kd
                  }   // rd kd
                }     // on tr
                if ((strlen(mrkd) != 0) && (strlen(nstkd1) != 0))
                { // compare
                  int ik = 0, ik1, iv2, iv1, fl = 0;
                  char str[5], strot[5];
                  while (ik < strlen(mruz2))
                  {
                    ik1 = 0;
                    while ((mruz2[ik] != ',') && (mruz2[ik] != '.') && (ik < strlen(mruz2)))
                    {
                      str[ik1] = mruz2[ik];
                      ik++;
                      ik1++;
                    }
                    str[ik1] = 0;
                    if ((mruz2[ik] == ',') || (mruz2[ik] == '.'))
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
          if (strlen(sss) == 0)
          {
            ivk++;
            continue;
          }
          int ik, fl1 = 0, fl2 = 0, ik1, iv1, iv2;
          char str[5], strot[5];
          for (ivs = 0; ivs < koluz; ivs++)
          { // for5
            ssss[0] = 0;
            sssss[0] = 0;
            fl1 = 0;
            fl2 = 0;
            for (int kk = 1; kk <= 4; kk++)
            { // for4
              mruz1[0] = 0;
              mruz2[0] = 0;
              mruz3[0] = 0;
              if (kk == 1)
              {
                strcpy(mruz1, uzel[iv].nmu0);
                strcpy(mruz2, uzel[ivk].nmu0);
                strcpy(mruz3, uzel[ivs].nmu0);
                if ((strlen(mruz1) != 0) && (strlen(mruz3) != 0))
                  strcat(ssss, "*");
                if ((strlen(mruz2) != 0) && (strlen(mruz3) != 0))
                  strcat(sssss, "*");
              }
              else if (kk == 2)
              {
                strcpy(mruz1, uzel[iv].nmu1);
                strcpy(mruz2, uzel[ivk].nmu1);
                strcpy(mruz3, uzel[ivs].nmu1);
              }
              else if (kk == 3)
              {
                strcpy(mruz1, uzel[iv].nmu2);
                strcpy(mruz2, uzel[ivk].nmu2);
                strcpy(mruz3, uzel[ivs].nmu2);
              }
              else if (kk == 4)
              {
                strcpy(mruz1, uzel[iv].nmu3);
                strcpy(mruz2, uzel[ivk].nmu3);
                strcpy(mruz3, uzel[ivs].nmu3);
              }
              if (kk != 1)
              { // 1
                ik = 0;
                while (ik < strlen(mruz1))
                { // 2
                  ik1 = 0;
                  while ((mruz1[ik] != ',') && (mruz1[ik] != '.') && (ik < strlen(mruz1)))
                  {
                    str[ik1] = mruz1[ik];
                    ik++;
                    ik1++;
                  }
                  str[ik1] = 0;
                  if ((mruz1[ik] == ',') || (mruz1[ik] == '.'))
                    ik++;
                  iv2 = 0;
                  while (iv2 < strlen(mruz3))
                  { // 4
                    iv1 = 0;
                    while ((mruz3[iv2] != ',') && (mruz3[iv2] != '.') && (iv2 < strlen(mruz3)))
                    {
                      strot[iv1] = mruz3[iv2];
                      iv2++;
                      iv1++;
                    }
                    strot[iv1] = 0;
                    if ((mruz3[iv2] == ',') || (mruz3[iv2] == '.'))
                      iv2++;
                    if ((strcmp(str, strot)) == 0)
                    { // 5
                      if (fl1 == 0)
                      {
                        if (kk == 2)
                          strcat(ssss, "%");
                        else if (kk == 3)
                          strcat(ssss, "@");
                        else if (kk == 4)
                          strcat(ssss, "&");
                        fl1 = 1;
                      }
                      else
                        strcat(ssss, ",");
                      strcat(ssss, str);
                    } // 5
                  }   // 4
                }     // 2
                ik = 0;
                while (ik < strlen(mruz2))
                { // 3
                  ik1 = 0;
                  while ((mruz2[ik] != ',') && (mruz2[ik] != '.') && (ik < strlen(mruz2)))
                  {
                    str[ik1] = mruz2[ik];
                    ik++;
                    ik1++;
                  }
                  str[ik1] = 0;
                  if ((mruz2[ik] == ',') || (mruz2[ik] == '.'))
                    ik++;
                  iv2 = 0;
                  while (iv2 < strlen(mruz3))
                  { // 44
                    iv1 = 0;
                    while ((mruz3[iv2] != ',') && (mruz3[iv2] != '.') && (iv2 < strlen(mruz3)))
                    {
                      strot[iv1] = mruz3[iv2];
                      iv2++;
                      iv1++;
                    }
                    strot[iv1] = 0;
                    if ((mruz3[iv2] == ',') || (mruz3[iv2] == '.'))
                      iv2++;
                    if ((strcmp(str, strot)) == 0)
                    { // 6
                      if (fl2 == 0)
                      {
                        if (kk == 2)
                          strcat(sssss, "%");
                        else if (kk == 3)
                          strcat(sssss, "@");
                        else if (kk == 4)
                          strcat(sssss, "&");
                        fl2 = 1;
                      }
                      else
                        strcat(sssss, ",");
                      strcat(sssss, str);
                    } // 6
                  }   // 44
                }     // 3
              }       // 1
              if ((strlen(sss) != 0) && (strlen(ss) != 0) && (strlen(ssss) != 0) && (strlen(sssss) != 0))
              { // данные в структуру
                float dw = sqrt(pow((uzel[iv].x - xot), 2) +  pow((uzel[iv].y - yot), 2)) + 
                  sqrt(pow((uzel[ivk].x - xkd), 2) +  pow((uzel[ivk].y - ykd), 2)) + 
                  sqrt(pow(uzel[iv].x - uzel[ivs].x, 2) + pow(uzel[iv].y - uzel[ivs].y, 2)) + 
                  sqrt(pow(uzel[ivs].x - uzel[ivk].x, 2) + pow(uzel[ivs].y - uzel[ivk].y, 2));
                if (dw < ps[0].d)
                {
                  strcpy(nmot1, ss);
                  strcpy(ost, nstot);
                  strcpy(ps[0].stperes1, uzel[ivs].suzel);
                  strcpy(ps[0].stperes2, uzel[ivk].suzel);
                  strcpy(ps[0].kd, nstkd);
                  strcpy(ps[0].ot, uzel[iv].suzel);
                  strcpy(ps[0].nmot, ssss);
                  strcpy(ps[0].nmkd, sss);
                  strcpy(ps[0].nmuz, sssss);
                  ps[0].d = dw;
                  j2++;
                }
              } // данные в структуру
            }   // for4
          }     // for5
          ivk++;
        } // two
      }   // откуда есть в узле
      iv++;
    } //
  }   // on uzel
  delete[] peres;
  delete[] kudan;
  delete[] otkudan;
  return j2;
}