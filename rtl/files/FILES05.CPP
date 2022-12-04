#include <io.h>
#include "files.hpp"

int Files::writefrombuf(unsigned _start, unsigned _len)
{
  if ((start <= _start) && (_start < end))
  {
    int cop = end > _start + _len ? _start + _len : end;
    if (seek(_start) == -1)
      return -1;
    return ::write(handle, buf + (_start - start) * recsize, (cop - _start) * recsize) != -1
               ? cop - _start
               : -1;
  }
  return 0;
}
