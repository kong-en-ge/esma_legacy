#include <errno.h>
#include <fcntl.h>
#include <io.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <alloc.h>

#include "buffiles.hpp"
#include "xms.h"

XMSFiles::XMSFiles() : BufFiles()
{
  hanxms = 0;
}

XMSFiles::~XMSFiles(void)
{
  close();
}

int XMSFiles::open(const char far *path)
{
  int ret;
  ret = BufFiles::open(path);
  if (ret != 0)
    return ret;
  if (XMSinstalled())
  {
    long l = filelength(getHandle()), l1;
    unsigned long copy = 0;
    unsigned bs = (bufsize * recsize) & 0xfffe;
    unsigned xmsm, mm;
    int c;
    l1 = (l + 1023) >> 10;
    getXMSmem(&xmsm, &mm);
    if (l1 > mm)
      goto noxms;
    hanxms = allocXMS(l1);
    if (hanxms == 0)
      goto noxms;
    lseek(getHandle(), 0, SEEK_SET);
    while (copy < l)
    {
      unsigned r;
      EMMSTRUC st;
      r = ::read(getHandle(), buf, bs);
      if (r == 0xffff)
        goto noxms;
      st.size = r;
      st.handles = 0;
      st.ptrs = buf;
      st.handled = hanxms;
      st.smesd = copy;
      moveXMS(&st);
      if (XMSerror() != 0)
        goto noxms;
      copy += r;
    }
    return 0;
  }
noxms:
  if (hanxms)
    freeXMS(hanxms);
  hanxms = 0;
  return 0;
};

int XMSFiles::close(void)
{
  if (hanxms)
    freeXMS(hanxms);
  hanxms = 0;
  return BufFiles::close();
}

int XMSFiles::makebuf(unsigned long _start, unsigned _len, void far *_buf)
{
  if (hanxms == 0)
    return BufFiles::makebuf(_start, _len, _buf);
  EMMSTRUC st;
  _len = (_len + 1) & 0xfffe;
  st.size = _len;
  st.handles = hanxms;
  st.smess = (_start * recsize) + headsize;
  st.handled = 0;
  st.ptrd = _buf;
  moveXMS(&st);
  if (XMSerror() != 0)
    return -1;
  else
    return 0;
}

int XMSFiles::readhead(void *_buf)
{
  if (hanxms == 0)
    return BufFiles::readhead(_buf);
  EMMSTRUC st;
  st.size = headsize & 0xfffe;
  st.handles = hanxms;
  st.smess = 0;
  st.handled = 0;
  st.ptrd = _buf;
  moveXMS(&st);
  if (XMSerror() != 0)
    return -1;
  else
    return 0;
}
