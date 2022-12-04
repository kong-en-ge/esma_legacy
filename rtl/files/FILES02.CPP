#include <io.h>
#include "files.hpp"

int Files::writehead(const void *_buf)
{
  int ret;

  if ((ret = ::lseek(handle, 0, SEEK_SET)) != -1)
    ret = ::write(handle, _buf, headsize);

  return ret;
}
