#include <string.h>

#include "fatr.hpp"
#include "globals.hpp"
#include "flibrt.hpp"

unsigned findobj(FAST_MODEL &mod, FINDatrf &fa)
{
  unsigned numobj, size, ds;
  unsigned char len;
  int last, bool;
  union
  {
    char far *start_c;
    unsigned far *start_u;
    float far *start_f;
  };
  if (fa.blok < mod.hm.start || fa.blok >= mod.hm.end)
  {
  label_0:
    if (mod.hm.read(fa.blok, fa.blok + mod.hm.bufsize - 1,
                    (void **)&start_c) == -1)
      return 0xffff;
  }
  start_c = mod.hm.buf + (fa.blok - mod.hm.start) * mod.hm.recsize + fa.smes * 2;
  size = (mod.hm.end - fa.blok) * mod.hm.recsize - fa.smes * 2;
  do
  {
    if (size < 512 && mod.hm.filelen > mod.hm.end)
    {
      ds = start_c - mod.hm.buf;
      fa.blok = mod.hm.start + ds / mod.hm.recsize;
      fa.smes = (ds % mod.hm.recsize) / 2;
      goto label_0;
    }
    if (start_u[0] != 0xC018)
      return 0xFFFE; //нет разделителя
    numobj = start_u[1];
    if (numobj == 0xC018)
      return 0xFFFF; //последний разделитель
    start_u += 2;
    size -= 4;
    for (int i = 0; i < fa.natr; i++)
      fa.atr[i].tpr = 0;
    do
    {
      char c = start_c[1];
      last = (len = (c & 0x7F)) != c;
      for (i = 0; i < fa.natr; i++)
        if (start_c[0] == fa.atr[i].natr)
        {
          switch (ATRN[start_c[0]].type)
          {
          case 'T':
            fa.atr[i].tpr = !strncmpi(start_c + 2, fa.atr[i].zns,
                                      strlen(fa.atr[i].zns));
            break;
          case 'K':
            fa.atr[i].tpr = start_u[1] == fa.atr[i].znk;
            break;
          case 'N':
            fa.atr[i].tpr = *((float *)(start_c + 2)) == fa.atr[i].znn;
            break;
          }
          break;
        }
      size -= (len + 2);
      start_c += len + 2;
    } while (!last);
    bool = 1;
    for (i = 0; i < fa.natr; i++)
      bool &= fa.atr[i].tpr;
  } while (!bool);
  ds = start_c - mod.hm.buf;
  fa.blok = mod.hm.start + ds / mod.hm.recsize;
  fa.smes = (ds % mod.hm.recsize) / 2;
  return numobj;
}

int make_atr(mk_atrf *m_a)
{
  m_a->natr = (unsigned char)*m_a->har;
  int l = *(m_a->har + 1) & 0x7f;
  int ret = (*(m_a->har + 1) & 0x80) >> 7;
  m_a->har += 2;
  memmove(m_a->zns, m_a->har, l);
  m_a->har += l;
  return ret;
}
