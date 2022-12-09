#include <stdio.h>
#include <math.h>
#include <string.h>

#include "flibrt.hpp"
#include "fatr.hpp"
#include "globalda.hpp"
#include "apputils.hpp"

int poisk(int regim = 0, int *x = NULL, int *y = NULL, double *b = NULL);

int otk_met_kuda(PERES *pss, OBSTOP *otkd, int retkdot, int nn)
{
  OBSTOP *kudan, *peres;
  int r, ckot, ckkd, j2 = 0;
  mk_atrf maot, makd;
  F_SPR sprot, sprkd;
  char ss[55], ss1[55];
  char mrot[55], mrkd[55], nstot[40], nstkd[40];
  int ret, xmt, ymt, at, kl;

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
  if (model.readspr(&sprot, nn) == -1)
  {
    displayError(1);
    delete[] peres;
    delete[] kudan;
    return 0;
  }
  int xn = sprot.xmin, yn = sprot.ymin;
  int num, nm1 = 0, ob = 0;
  for (int k = 1; k <= 4; k++)
  { // for
    int ii = 0;
    while ((otkd[ii].vt != k) && (ii < retkdot))
      ii++;
    if (ii >= retkdot)
      num = -1;
    else
    {
      float dot = otkd[ii].d;
      num = ii;
      while (ii < retkdot)
      {
        if (otkd[ii].vt == k)
          if (otkd[ii].d < dot)
          {
            dot = otkd[ii].d;
            num = ii;
          }
        ii++;
      }
    }
    if (num != -1)
    {
      kudan[nm1] = otkd[num];
      ob++;
      nm1++;
    }
  } // for
  int iv = 0;
  if (kolmet != 0)
  { // on metro
    while (iv < kolmet)
    { //
      xmt = metro[iv].x;
      ymt = metro[iv].y;
      int retmt = poisk(2, &xmt, &ymt);
      if (!retmt)
      {
        iv++;
        continue;
      }
      at = 0;
      kl = 0;
      do
      {
        if (obstop[at].vt != 1)
        {
          peres[kl] = obstop[at];
          kl++;
        }
        at++;
      } while (at < retmt);
      retmt = kl;
      for (int k = 1; k <= 4; k++)
      { // for2
        int iot, ikd;
        iot = 0;
        ikd = 0;
        while ((kudan[ikd].vt != k) && (ikd < ob))
          ikd++;
        while ((peres[iot].vt != k) && (iot < retmt))
          iot++;
        if ((iot < retmt) && (ikd < ob))
        {
          ckot = iot;
          ckkd = ikd;
          while (ckot < retmt)
          { // wh
            mrot[0] = 0;
            nstot[0] = 0;
            if (peres[ckot].vt == k)
            { // on transport
              int ret2 = model.readspr(&sprot, peres[ckot].nobj);
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
                    while (ckkd < ob)
                    { // wh kuda
                      mrkd[0] = 0;
                      nstkd[0] = 0;
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
                        if ((strlen(mrkd) != 0) && (strlen(nstkd) != 0) && (k != 1))
                        { // compare
                          int ik = 0, ik1, iv2, iv1;
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
                            iv2 = 0;
                            while (iv2 < strlen(mrkd))
                            {
                              iv1 = 0;
                              while ((mrkd[iv2] != ',') && (iv2 < strlen(mrkd)))
                              {
                                strkd[iv1] = mrkd[iv2];
                                iv2++;
                                iv1++;
                              }
                              strkd[iv1] = 0;
                              if (mrkd[iv2] == ',')
                                iv2++;
                              if ((strcmp(str, strkd)) == 0)
                              {
                                if (ss[0] != 0)
                                  strcat(ss, ",");
                                strcat(ss, str);
                              }
                            }
                          }
                        } // compare
                        float dw = sqrt(pow((metro[iv].x - sprkd.xmin), 2) + pow((metro[iv].y - sprkd.ymin), 2)) + sqrt(pow((metro[iv].x - xn), 2) + pow((metro[iv].y - yn), 2));
                        if (strlen(ss) != 0)
                        {
                          int js = 0;
                          while (js < j2)
                          {
                            if ((strcmp(pss[js].ot_kd, nstkd) == 0) &&
                                (strcmp(pss[js].nm, ss) == 0) && (dw > pss[js].d))
                              break;
                            js++;
                          }
                          if (js < j2)
                          {
                            ckkd++;
                            continue;
                          }
                          js = 0;
                          {
                            for (js = 0; js < j2; js++)
                            {
                              if (dw < pss[js].d)
                                break;
                            }
                            if (js >= 10)
                            {
                              ckkd++;
                              continue;
                            }
                            memmove(pss + js + 1, pss + js, (j2 - js) * sizeof(PERES));
                            strcpy(pss[js].stperes, metro[iv].smetro);
                            strcpy(pss[js].ot_kd, nstkd);
                            strcpy(pss[js].nm, ss);
                            pss[js].d = dw;
                            pss[js].vt = k;
                            if (j2 < 10)
                              j2++;
                          }
                        }
                      } // on tr
                      ckkd++;
                    } // wh kuda
                  }   // on stop where
                }     // atribute ot
              }       // rd  ot
            }         // on transport
            ckot++;
          } // wh
        }
      } // for2
      iv++;
    } //
  }   // on metro
  delete peres;
  delete kudan;
  return j2;
}