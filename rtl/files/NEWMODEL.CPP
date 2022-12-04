#include <errno.h>
#include <fcntl.h>
#include <io.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <alloc.h>
#include <string.h>
#include <stdio.h>
#include "GEOMODEL.HPP"
#include "NEWMODEL.HPP"

NEW_MODEL::NEW_MODEL()
{
  memset(this, 0, sizeof(NEW_MODEL));
  hsp = h.hmt = m.hmt = -1; // handles
}

NEW_MODEL::~NEW_MODEL()
{
  if (hsp != -1)
    close();
}

int NEW_MODEL::close()
{
  int ret;
  if (write(hsp, spbuf, ((nspr + 15) & 0xfff0) * 32) == -1)
    ret = -1;
  else
    ret = 0;
  ret &= flush_methar(&m);
  ret &= flush_methar(&h);
  ret &= ::close(hsp);
  ret &= ::close(m.hmt);
  ret &= ::close(h.hmt);
  if (spbuf != NULL)
    delete spbuf;
  if (m.mtbuf != NULL)
    delete m.mtbuf;
  if (h.mtbuf != NULL)
    delete h.mtbuf;
  memset(this, 0, sizeof(NEW_MODEL));
  hsp = m.hmt = h.hmt = -1; // handles
  return 0;
}

int NEW_MODEL::open(char *path, char *mod, int sps, int mts, int hrs)
{
  char patht[80];
  sprintf(patht, "%s.s%s", path, mod);
  int len = strlen(path) + 1;
  if ((spbuf = new SPR[spsize = sps]) == NULL)
    return -1;
  memset(spbuf, 0, sps * 32);
  if ((hsp = _creat(patht, FA_ARCH)) == -1)
    return -1;
  if (lseek(hsp, 512, SEEK_SET) == -1)
    return -1;
  patht[len] = 'm';
  if ((m.mtbuf = new int[(m.mtsize = mts) * 256]) == NULL)
    return -1;
  memset(m.mtbuf, 0, mts * 512);
  if ((m.hmt = _creat(patht, FA_ARCH)) == -1)
    return -1;
  patht[len] = 'h';
  if ((h.mtbuf = new int[(h.mtsize = hrs) * 256]) == NULL)
    return -1;
  memset(h.mtbuf, 0, hrs * 512);
  if ((h.hmt = _creat(patht, FA_ARCH)) == -1)
    return -1;
  return 0;
}

int NEW_MODEL::writeform(const void *form)
{
  if (lseek(hsp, 0, SEEK_SET) == -1)
    return -1;
  if (::write(hsp, form, 512) == -1)
    return -1;
  if (lseek(hsp, 0, SEEK_END) == -1)
    return -1;
  return 0;
}

int NEW_MODEL::writespr(const SPR *spr)
{
  memmove(spbuf + nspr, spr, 32);
  if (++nspr >= spsize)
  {
    nspr = 0;
    if (write(hsp, spbuf, spsize * 32) == -1)
      return -1;
    memset(spbuf, 0, spsize * 32);
  };
  return 0;
}

int NEW_MODEL::writenullspr()
{
  SPR spr;
  memset(&spr, 0, 32);
  return writespr(&spr);
}

int NEW_MODEL::writemetsgb(const void *metr, SPR *spr)
{
  gab_ram((void *)metr, spr);
  return write_metharnew(metr, &spr->stblm, &spr->enblm, &m);
}

int write_metharnew(const void *metr, unsigned *stblm,
                    unsigned *enblm, METHAR *m)
{
  unsigned len = size_methar(metr);
  *stblm = m->nmtd + m->nmtr / 256 + 1;
  if (len + m->nmtr > m->mtsize * 256)
  {
    if (write(m->hmt, m->mtbuf, (m->nmtr & 0xff00) * 2) == -1)
      return -1;
    m->nmtd += m->nmtr / 256;
    memmove(m->mtbuf, m->mtbuf + (m->nmtr & 0xff00), (m->nmtr & 0xff) * 2);
    m->nmtr &= 0xff;
  };
  memmove(m->mtbuf + m->nmtr, metr, len * 2);
  m->nmtr += len - 1;
  *enblm = m->nmtd + (m->nmtr - 2) / 256 + 1;
  return 0;
}

int flush_methar(METHAR *m)
{
  *(m->mtbuf + m->nmtr) = 0xc018;
  *(m->mtbuf + m->nmtr + 1) = 0xc018;
  int u1 = (m->nmtd & 1), u2 = (m->nmtr & 0x100) == 0;
  if (write(m->hmt, m->mtbuf, (m->nmtr & 0xff00) * 2 + ((u1 ^ u2) ? 1024 : 512)) == -1)
    return -1;
  return 0;
}

int NEW_MODEL::writemets(const void *metr, SPR *spr)
{
  return write_metharnew(metr, &(spr->stblm), &(spr->enblm), &m);
}

int NEW_MODEL::writehars(const void *hars, SPR *spr)
{
  unsigned a;
  return write_metharnew(hars, &(spr->stblh), &a, &h);
}

int copymetrtobuf(void *buff, void *metr)
{
  unsigned len;
  if ((len = size_methar(metr)) == 0xffff)
    return -1;
  memmove(buff, metr, size_methar(metr) * 2);
  return 0;
}
