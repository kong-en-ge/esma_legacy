#ifndef FWRITEHPP
#define FWRITEHPP

#include "coretype.hpp"

class FWRITE
{
public:
  char *buff;
  int handle;
  int rec_size;
  int buff_size;
  int head_size;
  int current;

  ~FWRITE();
  int open(PCSTR path, int h_size, int r_size, int b_size);
  int w_head(PCVOID ptr);
  int add(PVOID);
  int close(int n = 1);
};

#endif