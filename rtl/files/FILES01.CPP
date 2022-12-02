#include <io.h>
#include "files.hpp"
int pascal Files::readhead(void *_buf) {
  int ret;
  if ((ret=::lseek(handle,0,SEEK_SET))!=-1) {
    ret=::read(handle,_buf,headsize);
  };
  return ret;
};
