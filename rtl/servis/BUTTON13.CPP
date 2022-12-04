#include <ctype.h>
#include <dir.h>
#include <dos.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys\stat.h>
#include "mouse.hpp"
#include "bigpath.hpp"
#include "geomodel.hpp"
#include "tpedit.hpp"

int sortbaza(void const *s1, void const *s2)
{
  return strncmp((const char *)s1, (const char *)s2, 12);
}

Big_Menu_baza::Big_Menu_baza(int xl, int yt, int xr, int _sizep,
                             char *titl, int rem, char *bazap) : Big_Menu_fadd(xl, yt, xr, _sizep, 4000, 12, titl, "", rem)
{
  strcpy(bazapath, bazap);
  struct ffblk f;
  baza = activ = act = 0;
  char *d;
  char path[80];
  sprintf(path, "%s\\*.*", bazap);
  int done = findfirst(path, &f, FA_DIREC);
  if (done)
  {
    add("Нет геобаз");
    noneb = 1;
  }
  else
  {
    noneb = 0;
    while (done == 0)
    {
      if ((f.ff_attrib == FA_DIREC) && (f.ff_name[0] != '.'))
        add(strupr(f.ff_name));

      done = findnext(&f);
    }
  }
  qsort(items, maxn, 12, sortbaza);
}

int Big_Menu_baza::menu_key()
{
l1:
  char str[80];
  first_str = bazapath;
  int key = Big_Menu_fadd::menu_key();
  if (noneb)
    key = -1;
  if (key != -1)
  {
    char itm[13];
    memmove(itm, nitem(key), 12);
    itm[12] = 0;
    char *bazes = items;
    int max = maxn;
    int nactiv = activ;
    int nact = act;
    baza = 1;
    activ = 0;
    act = 0;
    maxn = 0;
    s_init(128);
    s_addall(128);
    char *nitems = new char[128 * 12];
    if (nitems == NULL)
      return -1;
    items = nitems;
    struct ffblk f;
    int nonem = 0;
    char path[80];
    sprintf(path, "%s\\%s\\%s.S*", bazapath, itm, itm);
    first_str = path;
    int done = findfirst(path, &f, FA_ARCH);
    *strrchr(path, '.') = 0;
    add("..");
    while (done == 0)
    {
      char *t, *w;
      w = t = strchr(f.ff_name, '.') + 1;
      while (w > f.ff_name)
      {
        *w = *(w - 1);
        w--;
      }
      *w = ' ';
      if (!isexistmodel(path, t + 1))
        add(strlwr(f.ff_name));
      done = findnext(&f);
      if (maxn > 127)
        break;
    }
    if (maxn <= 1)
    {
      add("Нет геобаз");
      nonem = 1;
    }
    else
    {
      qsort(items + 12, maxn - 1, 12, sortbaza);
      mark_models();
    }
    int key2 = Big_Menu_fadd::menu_key();
    delete SET::base;
    SET::base = NULL;
    if (nonem)
      key2 = 0;
    if (key2 > 0)
    {
      strncpyz(model, nitem(key2) + 1, 11);
    }
    delete nitems;
    items = bazes;
    maxn = max;
    activ = nactiv;
    act = nact;
    baza = 0;
    if (key2 <= 0)
      goto l1;
    return 0;
  }
  return -1;
}

void Big_Menu_baza::mark_models()
{
}