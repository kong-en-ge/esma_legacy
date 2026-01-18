#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <io.h>
#include <fcntl.h>
#include <dir.h>

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

/*
void ATRMT(TEXT_PACK_MENUS *tp,CSubwayTransferArray const &transfers)
{ // ATRMT
  char ss[80], ss1[80];
  sprintf(ss, "На метро до ст. %s", transfers[0].subwayTitle);
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
}

void ATRTM(CTransportModule const& transportModule, TEXT_PACK_MENUS *tp, int ob)
{
/*  char ss1[80];
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
  sprintf(ss1, "На метро до ст. %s", transportModule.getTargetSubway());
  tp->add(ss1);
  tp->add("         ***         ");
}

void ATRPER(TEXT_PACK_MENUS *tp, int ob)
{
/*  char ss1[100];
  for (int i = 0; i < ob; i++)
  { // for
    sprintf(ss1, "От ост. %s", psu[i].ot);
    tp->add(ss1);
    processRouteData(tp, psu[i].nmot);
    sprintf(ss1, "До ост.%s", psu[i].stperes);
    tp->add(ss1);

    tp->add("  ДАЛЕЕ  ");
    processRouteData(tp, psu[i].nmkd);
    sprintf(ss1, "До ост. %s", psu[i].kd);
    tp->add(ss1);
    tp->add("         ***        ");
  }
}

void ATR2PER(TEXT_PACK_MENUS *tp, int ob)
{
  char str[100];
  for (int i = 0; i < ob; i++)
  {
    sprintf(str, "От ост. %s", pstwo[i].ot);
    tp->add(str);
    processRouteData(tp, pstwo[i].nmot);
    sprintf(str, "До ост. %s", pstwo[i].stperes1);
    tp->add(str);

    tp->add("  ЗАТЕМ  ");
    processRouteData(tp, pstwo[i].nmuz);
    sprintf(str, "До ост. %s", pstwo[i].stperes2);
    tp->add(str);

    tp->add("  И НАКОНЕЦ  ");
    processRouteData(tp, pstwo[i].nmkd);
    sprintf(str, "До ост. %s", pstwo[i].kd);
    tp->add(str);
    tp->add("         ***        ");
  }
}
*/