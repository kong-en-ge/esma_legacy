#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include <stdlib.h>

#include "coretype.hpp"

bool findCommandLineParam(int argc, char *argv[], PCSTR param, PSTR value = NULL);
int strStartCmp(PCSTR str, PCSTR pattern);
int strStartCmpI(PCSTR str, PCSTR pattern);

#endif