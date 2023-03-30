#include <string.h>

#include "rbmaphn.hpp"
#include "globals.hpp"
#include "drawpl.hpp"
#include "fatr.hpp"
#include "funcs.hpp"

void dr_pn_n(F_SPR *spr, char *clcod, int color, int en, int cod)
{
  if (!en)
  {
    if ((CURRENTSCALE >= 12.0 * SCALEMIN) && (!strncmp(clcod, "412", 3)))
      return;
    if ((CURRENTSCALE >= 5.0 * SCALEMIN) && (!strncmp(clcod, "42", 2)))
      return;
    if ((CURRENTSCALE >= 10.0 * SCALEMIN) && (cod))
      return;
  }
  mk_atrf ma;
  if ((spr->sbh == 0) || (model.readhars((int **)&ma.har, spr) == -1))
    return;
  ma.har += 4;
  int r;
  char namstd[41], namst[41];
  namst[0] = 0;
  do
  {
    r = make_atr(&ma);
    if (checkAttributeName(ma.natr, "09"))
      strcpy(namst, ma.zns);
  } while (r == 0);
  if (!namst[0])
    return;
  if (!cod)
  {
    strcpy(namstd, namst);
    if (!strncmp(clcod, "41", 2))
      strupr(namstd);
  }
  else
  {
    if (cod == 1)
      strcpy(namstd, "оз. ");
    else
      strcpy(namstd, "вдхр. ");
    strcat(namstd, namst);
  }
  if (color == 12)
    ft.color = 12;
  else if (!cod)
    ft.color = BLACK;
  else
    ft.color = LIGHTBLUE;
  ft.font = 0;
  ft.size = 0;
  ft.gaps = 1;
  int mst[7];
  mst[4] = spr->xmin / 2 + spr->xmax / 2;
  mst[5] = spr->ymin / 2 + spr->ymax / 2;
  mst[6] = 0xc018;
  scr.outstring(namstd, mst, &ft);
}

/*********************
 * CRbMapDataHandler *
 *********************/

int CRbMapDataHandler::draw_one_obj(CViewContext const& viewContext, unsigned obj, 
  int color, int drawDisabled)
{
  F_SPR spr;
  if (model.readspr(&spr, obj) == -1)
    return -1;
  if ((!drawDisabled) && (!obj_enabled(viewContext, &spr)))
    return 0;
  FLIB *flib = flibrt + spr.numincl;
  int loc = flib->loc;
  if (color == -1)
    color = loc == 3 ? flib->colorg : flib->color;
  setcolor(color);
  if (loc == 1)
  {
    int j = flib->num - 1;
    unsigned mask;
    putmarker_16_n(xmapwin(spr.xmin) - ras[j].x,
                   ymapwin(spr.ymin) - ras[j].y, 16, ras[j].obr);
  }
  else if ((loc >= 2) && (loc <= 4))
  {
    // площади и линии и подписи
    int *metr;
    int j;
    if (model.readmets(&metr, &spr) == -1)
      return -1;
    if (FULL_MAP && loc == 3 && flib->num != 12)
    {
      setfillstyle(flib->num, flib->color);
      polygon(metr - 2);
    }
    else if (loc != 4)
    {
      drawingline(metr - 2, loc == 2 ? flib->num : 0);
    }
    else
    {
      struct MFT
      {
        unsigned font : 3;
        unsigned size : 3;
        unsigned gaps : 2;
      } mft;
      ft.color = color;
      memmove(&mft, &flib->num, 1);
      ft.font = mft.font;
      ft.size = mft.size;
      ft.gaps = mft.gaps;
      scr.outstring((char *)(metr + size_methar(metr) + 1), metr - 2, &ft);
    }
    if ((flib->clcod[0] == '4') && (loc == 3))
      dr_pn_n(&spr, flib->clcod, color, drawDisabled, 0);
    if (!strncmp(flib->clcod, "31120000", 8))
      dr_pn_n(&spr, flib->clcod, color, drawDisabled, 1);
    if (!strncmp(flib->clcod, "31131000", 8))
      dr_pn_n(&spr, flib->clcod, color, drawDisabled, 2);
  }
  return 0;
}
