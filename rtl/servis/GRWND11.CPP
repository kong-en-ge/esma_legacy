#include "grwnd.hpp"

// sym ᨬ����� �� n ��ப� c start
void GRwnd::clear_str(int n, int start, int sym)
{
  SVS svs;
  savesettings(&svs);
  setfillstyle(SOLID_FILL, WHITE);
  int y = n * 12 + top + 1 + BORD_S, x = left + BORD_S + 8 * start;
  bar(x, y, x + 8 * sym, y + 8);
  restoresettings(&svs);
}
