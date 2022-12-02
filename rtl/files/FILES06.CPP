#include <errno.h>
#include <io.h>
#include "mem.h"
#include "files.hpp"

int pascal Files::readtobuf(unsigned _start,unsigned _end,void *buf) {
  void  *bf;
  int ret;
  if ((ret=read(_start,_end,&bf))>0) memmove(buf,bf,recsize*ret);
  return ret;

}