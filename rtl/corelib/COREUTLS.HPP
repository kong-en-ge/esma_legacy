#ifndef __COREUTLS_HPP__
#define __COREUTLS_HPP__

#include "coretype.hpp"

int strStartCmp(PCSTR str, PCSTR pattern);
int strStartCmpI(PCSTR str, PCSTR pattern);

void memswap(PVOID item1, PVOID item2, unsigned size);
void reverseArray(PVOID base, unsigned elemSize, unsigned count);

#endif