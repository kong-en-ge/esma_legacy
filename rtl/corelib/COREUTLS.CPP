#include <string.h>

#include "coreutls.hpp"

int strStartCmp(PCSTR str, PCSTR pattern)
{
  return strncmp(str, pattern, strlen(pattern));
}

int strStartCmpI(PCSTR str, PCSTR pattern)
{
  return strncmpi(str, pattern, strlen(pattern));
}

void memswap(PVOID item1, PVOID item2, unsigned size)
{
  PBYTE src1 = (PBYTE)item1;
  PBYTE src2 = (PBYTE)item2;
  while (size-- > 1)
  {
    BYTE temp = *src1;
    *src1 = *src2;
    *src2 = temp;
    src1++;
    src2++;
  }
}

void reverseArray(PVOID base, unsigned elemSize, unsigned count)
{
  PBYTE item1 = (PBYTE)base;
  PBYTE item2 = item1 + elemSize * (count - 1);

  while (count > 1)
  {
    memswap(item1, item2, elemSize);
    item1 += elemSize;
    item2 += elemSize;
    count -= 2;
  }
}

