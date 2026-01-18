#include <assert.h>
#include <stdlib.h>

#include "softasrt.hpp"

static TRAP_FUNC gTrapFunc = NULL;

void __trap(char *__msg, char *__cond, char *__file, int __line)
{
  if (gTrapFunc)
    gTrapFunc(__msg, __cond, __file, __line);
  else
    __assertfail(__msg, __cond, __file, __line);
}

TRAP_FUNC setAssertTrap(TRAP_FUNC trapFunc)
{
  TRAP_FUNC retValue = gTrapFunc;
  gTrapFunc = trapFunc;
  return retValue;
}
