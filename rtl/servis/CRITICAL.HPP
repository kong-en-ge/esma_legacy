#ifndef CRITICALHPP
#define CRITICALHPP
#include "grwnd.hpp"

struct CRITICAL
{
  char *mem;

  CRITICAL();
  ~CRITICAL();
  void store();
  void restore();
};

extern CRITICAL HANDLER24;

#endif