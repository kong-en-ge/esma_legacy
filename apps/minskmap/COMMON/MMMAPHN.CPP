#include <string.h>

#include "mmmaphn.hpp"
#include "globals.hpp"
#include "funcs.hpp"
#include "fatr.hpp"
#include "drawpl.hpp"

int lin_wnd(int *, int *, int *, int *, int, int, int, int);

void dr_bl_n(int *metr, F_SPR *spr, char *clcod)
{
  if ((CURRENTSCALE >= 12.0 * SCALEMIN) || (strncmp(clcod, "44", 2)))
    return;
  int x = xmapwin(*(metr + 2));
  int y = ymapwin(*(metr + 3));
  if (((x - 1) < 0) || ((y - 9) < 0) || ((x + 8) > XMAXMAPE) || ((y + 1) > YMAXMAPE))
    return;
  mk_atrf ma;
  if ((spr->lnh == 0) || (model.readhars((int **)&ma.har, spr) == -1))
    return;
  ma.har += 4;
  int nbn = 0, r;
  char ncn = 0;
  do
  {
    r = make_atr(&ma);
    if (checkAttributeName(ma.natr, "BN"))
      nbn = ma.znn;
    if (checkAttributeName(ma.natr, "CN"))
      ncn = ma.zns[0];
  } while (r == 0);
  if (!nbn)
    return;
  char sbn[6];
  memset(sbn, 0, 6);
  itoa(nbn, sbn, 10);
  if (ncn)
  {
    sbn[strlen(sbn)] = '_';
    sbn[strlen(sbn)] = ncn;
  }
  if ((x + 8 * strlen(sbn)) > XMAXMAPE)
    return;
  setfillstyle(0, WHITE);
  bar(x - 1, y - 9, x + 8 * strlen(sbn), y + 1);
  outtextxy(x + 4 * strlen(sbn), y - 3, sbn);
}

void rotate_metr(int *metr)
{
  int size = size_methar(metr);
  asm mov cx, size
  asm mov ax, cx
  asm shr cx, 1
  asm shr cx, 1
  asm jz r_en
  asm sub ax, 3
  asm shl ax, 1
  asm les di, metr
  asm mov si, di
  asm add si, ax
  asm mov bx, 2
r_loop:
  asm mov ax, es : [di]
  asm xchg ax, es : [si]
  asm mov es : [di], ax
  asm mov ax, es : [di + bx]
  asm xchg ax, es : [si + bx]
  asm mov es : [di + bx], ax
  asm add di, 4
  asm sub si, 4
  asm loop r_loop
r_en:
  return;
}

void dr_st_n(CViewContext const& viewContext, F_SPR *spr, int *metr)
{
  mk_atrf ma;
  if ((spr->sbh == 0) || (model.readhars((int **)&ma.har, spr) == -1))
    return;
  ma.har += 4;
  int r;
  char namst[41];
  namst[0] = 0;
  do
  {
    r = make_atr(&ma);
    if (checkAttributeName(ma.natr, "ST"))
      strncpyz(namst, getLookupAttributeValue(ma.natr, ma.znk), 40);
  } while (r == 0);
  if (!namst[0])
    return;
  int mst[200], *mi = metr + 2, *mo = mst + 4;
  int bp = 1, x1, y1, x2, y2;
  int dx = xwinmap(8) - xwinmap(0);
  int dy = -ywinmap(8) + ywinmap(0);
  CRectS const& clipBounds = viewContext.getMapClipBounds();
  while (*(mi + 2) != 0xc018)
  {
    x1 = *mi;
    y1 = *(mi + 1);
    x2 = *(mi + 2);
    y2 = *(mi + 3);
    int fl = lin_wnd(&x1, &y1, &x2, &y2, clipBounds.left + dx, clipBounds.top + dy,
                     clipBounds.right - dx, clipBounds.bottom - dy);
    if (fl != -1)
    {
      if (bp)
      {
        *mo++ = x1;
        *mo++ = y1;
        bp = 0;
      }
      /*	 if (length(x2-*(mo-2),y2-*(mo-1))>=dx) */ {
        *mo++ = x2;
        *mo++ = y2;
      }
      if (fl)
        break;
    }
    else if (!bp)
      break;
    mi += 2;
  }
  if (mo == (mst + 4))
    return;
  if (length(*(mst + 4) - *(mo - 2), *(mst + 5) - *(mo - 1)) < 8 * dx)
    return;
  *mo = 0xc018;
  if (*(mst + 4) > *(mo - 2))
    rotate_metr(mst + 4);
  ft.color = BLACK;
  ft.font = 0;
  ft.size = 0;
  ft.gaps = 1;
  strupr(namst);
  // setlinestyle(0,0,2);
  // setcolor(12);
  // drawingline(mst,1);
  scr.outstring(namst, mst, &ft);
}

/************************
 * CMinskMapDataHandler *
 ************************/

CMinskMapDataHandler::CMinskMapDataHandler(void)
{
  displayNumbers = 0;
}

int CMinskMapDataHandler::draw_one_obj(CViewContext const& viewContext, unsigned obj, 
  int color, int drawDisabled)
{
  F_SPR spr;
  if (model.readspr(&spr, obj) == -1)
    return -1;
  if (!drawDisabled && !obj_enabled(viewContext, &spr))
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
    //площади и линии и подписи
    int *metr;
    int j;
    if (model.readmets(&metr, &spr) == -1)
      return -1;
    if (FULL_MAP && loc == 3 && flib->num != 12)
    {
      setfillstyle(flib->num, flib->color);
      polygon(metr - 2);
      if (displayNumbers)
        dr_bl_n(metr, &spr, flib->clcod);
    }
    else if (loc != 4)
    {
      drawingline(metr - 2, loc == 2 ? flib->num : 0);
      if ((FULL_MAP) && (SCALES < 118) &&
          (!strncmp(flib->clcod, "45", 2)) && (loc == 2))
        dr_st_n(viewContext, &spr, metr);
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
  }
  return 0;
}