#include <stdio.h>
#include "button.hpp"

void Big_Menu_XY::draw_one(int num)
{
  char str[80];
  sprintf(str, "%4i  %8g  %8g", activ + num, *(xptr + activ + num), *(yptr + activ + num));
  draw_one_str(str, num);
}

Big_Menu_XY::Big_Menu_XY(int xl, int yt, int xr, int sizep_, float *X,
                         float *Y, int Maxn, char const *titl, char const *first_str, int rem, int frst_str, int end_str) : Big_Menu(xl, yt, xr, sizep_, titl, first_str, rem, frst_str, end_str)
{
  maxn = Maxn;
  xptr = X;
  yptr = Y;
}
