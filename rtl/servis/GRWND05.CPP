#include <string.h>
#include "grwnd.hpp"

void GRwnd::text(char const *string, int center)
{
  SVS svs;
  savesettings(&svs);
  setcolor(BLACK);
  int x;
  if (center)
  {
    settextjustify(LEFT_TEXT, TOP_TEXT);
    x = left + 2 * BORD_S;
  }
  else
  {
    settextjustify(CENTER_TEXT, TOP_TEXT);
    x = (left + right) / 2;
  };
  int len = (right - left - 4 * BORD_S) >> 3;
  char str2[80];
  str2[len] = 0;
  int b = 1, k, e;
  do
  {
    e = 0;
    if (len >= strlen(string))
    {
      b = 0;
      k = strlen(string);
    }
    else
    {
      k = len;
      while ((*(string + k) != ' ') && (k > 0))
        k--;
      if (k == 0)
        k = len;
      else
        e = 1;
    };
    strncpy(str2, string, k);
    str2[k] = 0;
    outtextxy(x, (nst++) * 12 + top + 1 + BORD_S, str2);
    string += k + e;
  } while (b);
  restoresettings(&svs);
}
