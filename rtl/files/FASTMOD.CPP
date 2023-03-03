#include <stdio.h>
#include <string.h>
#include "fastmod.hpp"

int FAST_MODEL::open(const char far *name, const char far *model)
{
  char patht[80];
  sprintf(patht, "%s.c%s", name, model);
  int len = strlen(name) + 1;
  hs.Init(sizeof(F_FORM), sizeof(F_SPR), 2000);
  if (hs.open(patht) == -1)
    return -1;
  if (hs.readhead(&fm) == -1)
    return -1;
  patht[len] = 'd';
  hm.Init(0, 512, 100);
  if (hm.buf != NULL)
  {
    int *b = (int *)hm.buf;
    *(b + 100 * 256) = 0xC018;
  };
  if ((hm.open(patht)) == -1)
    return -1;
  hh.Init(0, 512, 40);
  if (hh.buf != NULL)
  {
    int *b = (int *)hh.buf;
    *(b + 40 * 256) = 0xC018;
    hh.handle = hm.getHandle();
    hh.filelen = hm.filelen;
    hh.hanxms = hm.hanxms;
  };
  return 0;
}

int FAST_MODEL::readmets(int **metp, F_SPR *spr)
{
  int *m, s = spr->sbm;
  int e = s + (spr->lnm + spr->ssm - 2) / 256;
  int ret = hm.read(s, e, (void **)&m);
  *metp = m + spr->ssm;
  return ret;
}

int FAST_MODEL::readhars(int **harp, F_SPR *spr)
{
  int *m, s = spr->sbh;
  int e = s + ((spr->lnh + spr->ssh - 2) / 256);
  int ret = hh.read(s, e, (void **)&m);
  *harp = m + spr->ssh;
  return ret;
}

int FAST_MODEL::close()
{
  hh.handle = -1;
  hh.buf = 0;
  hh.hanxms = 0;
  int ret = hs.close();
  return ret | hm.close();
}

FAST_MODEL::~FAST_MODEL()
{
  close();
}
