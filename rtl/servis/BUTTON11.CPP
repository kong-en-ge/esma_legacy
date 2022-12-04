#include <errno.h>
#include <dir.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bigpath.hpp"
#include "geomodel.hpp"
#include "tpedit.hpp"
#include "disks.hpp"
#include "critical.hpp"

int FLOPPY_DRIVE;

void near s()
{
  HANDLER24.store();
}

Big_Menu_path::Big_Menu_path(int xl, int yt, int xr, int _sizep,
                             char *titl, char *path, int rem) : Big_Menu_fadd(xl, yt, xr, _sizep, 4000, 13, titl, "", rem)
{
  tt = 0;
  char ext[5];
  char startd[3];
  int len;
  fnsplit(strupr(path), startd, startdir, maskfile, ext);
  len = strlen(startdir);
  if (len)
    startdir[len - 1] = 0;
  if (startd[0] == 0)
    startdisk = getdisk();
  else
    startdisk = startd[0] - 'A';
  strcat(maskfile, ext);
}

int sorting(void const *s1, void const *s2)
{
  return strncmp((const char *)s1, (const char *)s2, 12);
}

int addisk(Big_Menu_path *mn)
{
  char str[14];
  int n = 0;
  int MaxDisk = setdisk(getdisk());
  for (int i = 1; i <= MaxDisk; i++)
  {
    int ret = is_disk_valid(i);
    if ((ret == -1) || ((ret == 0) && (FLOPPY_DRIVE == 0)))
      continue;
    sprintf(str, "[%c:]", 'A' + i - 1);
    str[12] = 0;
    mn->add(str);
    n++;
  }
  return n;
}

int Big_Menu_path::init()
{
  struct ffblk f;
  char str[14];
  maxn = activ = 0;
  char *d;
  int n = 0;
  n = addisk(this);
  if (tt)
    d = strupr(strrchr(path, '\\')) + 1;
  activ = n >= sizep ? n - sizep + 1 : 0;
  act = n - activ;
  int done = findfirst("*", &f, FA_DIREC);
  while (done == 0)
  {
    if ((f.ff_attrib == FA_DIREC) && strcmp(f.ff_name, "."))
    {
      strcpy(str, strupr(f.ff_name));
      str[12] = 1;
      add(str);
    }
    done = findnext(&f);
  }
  qsort(items + 13 * n, maxn - n, 13, sorting);
  if ((errno != ENOENT) && (errno == EMFILE))
    return 0;
  if (tt)
  {
    for (int i = 0; i < maxn; i++)
      if (!strcmp(d, nitem(i)))
      {
        activ = i >= sizep ? i - sizep + 1 : 0;
        act = i - activ;
      }
    tt = 0;
  }
  n = maxn;
  int ret = initf();
  // qsort(items+13*n,maxn-n,13,sorting);
  return ret;
}

int Big_Menu_path::initf()
{
  struct ffblk f;
  char str[13];
  int done = findfirst(maskfile, &f, FA_ARCH);
  while (done == 0)
  {
    strcpy(str, strlwr(f.ff_name));
    str[12] = 2;
    add(str);
    done = findnext(&f);
  }
  return 0;
}

int Big_Menu_path::menu_key()
{
  textwidth = 12;
  int key, ds;
  char olddir[80], fstr[80];
  int olddisk = getdisk();
  setdisk(startdisk);
  olddir[0] = '\\';
  if (getcurdir(0, olddir + 1) == -1)
  {
    maxn = 0;
    addisk(this);
    olddir[0] = 0;
  }
  else
  {
    if (olddir[1] == '\\')
      olddir[1] = 0;
    chdir(startdir);
    if (!maxn)
      init();
  }
l1:
  path[0] = startdisk + 'A';
  strcpy(path + 1, ":\\");
  if (olddir[0] == 0)
    strcat(path, " ? ");
  else
  {
    getcurdir(0, path + 3);
    if (!path[3])
      path[2] = 0;
  }
  sprintf(fstr, "%s\\%s", path, maskfile);
  fstr[(GRwnd::right - GRwnd::left - 6 * BORD_S) / 8] = 0;
  first_str = fstr;
l2:
  key = Big_Menu::menu_key();
  if (key != -1)
  {
    char _path[80];
    char *it = nitem(key);
    if (*(it + 12) == 0)
    { // Выбран диск
      ds = *(it + 1) - 'A';
      if (olddir[0] != 0)
        chdir(olddir);
      if (setdisk(startdisk = ds) == -1)
      {
        key = -1;
        goto lex;
      };
      olddir[0] = '\\';
      if (getcurdir(0, olddir + 1) == -1)
      {
        olddir[0] = 0;
        maxn = 0;
        addisk(this);
        goto l1;
      };
      if (olddir[1] == '\\')
        olddir[1] = 0;
      goto l3;
    }
    else if (*(it + 12) == 1)
    {
      strncpyz(_path, nitem(key), 12);
      if (chdir(_path) == -1)
      {
        key = -1;
        goto lex;
      }
      if (!strcmp(_path, ".."))
        tt = 1;
    l3:
      init();
      goto l1;
    }
  }
lex:
  if (key != -1)
  {
    startdir[0] = '\\';
    getcurdir(0, startdir + 1);
    if (startdir[1] == '\\')
      startdir[1] = 0;
  }
  int len = strlen(path);
  path[len] = '\\';
  strncpyz(path + len + 1, nitem(key), 12);
  if (olddir[0] != 0)
    chdir(olddir);
  setdisk(olddisk);
  return key;
}
