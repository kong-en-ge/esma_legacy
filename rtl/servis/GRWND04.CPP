#include "grwnd.hpp"
#include "tpedit.hpp"

void GRwnd::draw(void)
{
  char OS = SYNC;
  SYNC = 1;
  SVS svs;
  savesettings(&svs);
  store();
  nst = 1;
  REGION r, r0;
  r = *(REGION *)&left;
  r0 = *(REGION *)&left;
  int top10b = (r0.top += BORD_S) + 10;
  setfillstyle(1, LIGHTGRAY);
  bar(r.left, r.top, r.right, r.bot);
  setfillstyle(1, WHITE);
  bar(r.left += BORD_S, top10b, r.right -= BORD_S, r0.bot -= BORD_S);
  setfillstyle(1, titlec);
  bar(r.left, r0.top, r.right, top10b);
  setcolor(YELLOW);
  settextjustify(CENTER_TEXT, CENTER_TEXT);
  outtextxy((r.left + r.right) >> 1, r.top + 5 + BORD_S, title);
  setcolor(WHITE);
  rectangle(r0.left, r.top, ++r0.right, r.bot + 1);
  setcolor(BLACK);
  rectangle(r0.left - 1, r.top - 1, r0.right, r.bot + 1);
  rectangle(r.left, r0.top - 1, r.right, top10b);
  rectangle(r.left, top10b, r.right, r0.bot + 1);
  restoresettings(&svs);
  SYNC = OS;
}
