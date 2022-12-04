#include <memory.h>
#include "button.hpp"

Big_Menu_file::Big_Menu_file(int xl, int yt, int xr, int sizep_, void *items_,
                             int Maxn, int Maxlen, char *titl, char *first_str, int rem, int frst_str, int end_str) : Big_Menu(xl, yt, xr, sizep_, titl, first_str, rem, frst_str, end_str)
{
  items = (char *)items_;
  maxn = Maxn;
  maxlen = Maxlen;
}

void Big_Menu_file::draw_one(int num)
{
  char string[80];
  memmove(string, items + maxlen * (activ + num), maxlen);
  string[maxlen] = 0;
  draw_one_str(string, num);
}
