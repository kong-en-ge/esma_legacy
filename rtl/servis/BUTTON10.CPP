#include <memory.h>
#include "button.hpp"

Big_Menu_fadd::Big_Menu_fadd(int xl, int yt, int xr, int sizep_, int Maxn,
                             int Maxlen, char *titl, char *first_str, int rem, int frst_str, int end_str) : Big_Menu(xl, yt, xr, sizep_, titl, first_str, rem, frst_str, end_str)
{
  items = new char[Maxn * Maxlen];
  maxlen = Maxlen;
}

Big_Menu_fadd::~Big_Menu_fadd()
{
  delete items;
}

int Big_Menu_fadd::add(char *item)
{
  if (items == NULL)
    return -1;
  memmove(items + maxlen * (maxn++), item, maxlen);
  return 0;
}

char *Big_Menu_fadd::nitem(int n)
{
  return items + maxlen * n;
}

void Big_Menu_fadd::del(int item)
{
  if (items == NULL)
    return;
  if (maxn > 1)
    maxn--;
  else
    return;
  memmove(nitem(item), nitem(item + 1), (maxn - item) * maxlen);
  if ((activ > 0) && (activ > maxn - sizep))
    activ--;
  else if ((act > 0) && (act + activ >= maxn))
    act--;
}

void Big_Menu_fadd::draw_one(int num)
{
  char string[80];
  memmove(string, items + maxlen * (activ + num), maxlen);
  string[maxlen] = 0;
  draw_one_str(string, num);
}
