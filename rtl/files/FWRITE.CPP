#include <io.h>
#include <memory.h>
#include <dos.h>

#include "FWRITE.HPP"

FWRITE::~FWRITE()
{
  if (handle != -1)
    close();
  if (!buff)
    delete buff;
}

int FWRITE::open(char const *path, int h_size, int r_size, int b_size)
{
  buff = NULL;
  current = 0;
  if ((handle = _creat(path, FA_ARCH)) == -1)
    return -1;
  if (lseek(handle, (head_size = h_size), 0) == -1)
    return -1;
  buff = new char[(rec_size = r_size) * (buff_size = b_size)];
  if (!buff)
    return -1;
  memset(buff, 0, rec_size * buff_size);
  return 0;
}

int FWRITE::w_head(void const *ptr)
{
  if (lseek(handle, 0, SEEK_SET) == -1)
    return -1;
  if (::write(handle, ptr, head_size) == -1)
    return -1;
  if (lseek(handle, 0, SEEK_END) == -1)
    return -1;
  return 0;
}

int FWRITE::add(void *ptr)
{
  memmove(buff + current * rec_size, ptr, rec_size);
  if (++current >= buff_size)
  {
    current = 0;
    if (write(handle, buff, rec_size * buff_size) == -1)
      return -1;
    memset(buff, 0, rec_size * buff_size);
  };
  return 0;
}

int FWRITE::close(int n)
{
  int len = (current + n - 1) & (-n);
  int ret;
  if (write(handle, buff, len * rec_size) == -1)
    ret = -1;
  else
    ret = 0;
  if (::close(handle) == -1)
    ret = -1;
  handle = -1;
  delete buff;
  buff = NULL;
  return ret;
}
