#include "grwnd.hpp"

void GRwnd::clear(void)
{
  SVS svs;
  savesettings(&svs);
  setfillstyle(1, WHITE);
  nst = 1;
  bar(left + 1 + BORD_S, top + 11 + BORD_S, right - 1 - BORD_S, bot - BORD_S);
  restoresettings(&svs);
}
