#include <errno.h>
#include <io.h>
#include <stdlib.h>
#include <string.h>

#include "files.hpp"

int Files::seek(unsigned no)
{
  long pos = headsize + ulongmul(no, recsize);
  int ret = ::lseek(handle, pos, SEEK_SET);
  return ret;
}

int Files::read(unsigned _start, unsigned _end, void **_buf)
{
  if (buf == NULL)
  {
    errno = ENOMEM;
    return -1;
  };
  unsigned st, en, smb = 0;
  if (_start >= filelen)
    return 0;
  if (_end >= filelen)
    _end = filelen - 1;
  if ((_start < start) || (end <= _end))
  { //отсутствует в буфуре
    en = _start + bufsize > filelen ? filelen : _start + bufsize;
    st = en > bufsize ? en - bufsize : 0;
    if ((st >= start) && (end > st))
    {
      smb = (end - st) * recsize;
      memmove(buf, buf + (st - start) * recsize, smb);
      start = st;
      st = end;
      end = en;
    }
    else if ((st < start) && (en <= end) && (en > start))
    {
      memmove(buf + (end - en) * recsize, buf, (en - start) * recsize);
      end = en;
      en = start;
      start = st;
    }
    else
    {
      start = st;
      end = en;
    };
    if ((seek(st) == -1) || (::read(handle, buf + smb, (en - st) * recsize) == -1))
    {
      flush();
      return -1;
    };
  }
  *_buf = buf + (_start - start) * recsize;
  return _end - _start + 1;
}
