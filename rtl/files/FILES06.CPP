#include <errno.h>
#include <io.h>
#include <memory.h>

#include "files.hpp"

int Files::readtobuf(unsigned _start, unsigned _end, void *buf)
{
  void *bf;
  int ret;
  if ((ret = read(_start, _end, &bf)) > 0)
    memmove(buf, bf, recsize * ret);
  return ret;
}