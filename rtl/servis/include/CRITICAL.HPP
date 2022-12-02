#ifndef CRITICALHPP
 #define CRITICALHPP
#include "grwnd.hpp"
struct CRITICAL{
  char *mem;
  CRITICAL();
  ~CRITICAL();
  void pascal store();
  void pascal restore();
};

extern CRITICAL HANDLER24;
#endif