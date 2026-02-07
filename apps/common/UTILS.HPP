#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include <stdlib.h>

#include "coretype.hpp"

bool findCommandLineParam(int argc, char *argv[], PCSTR param, PSTR value = NULL);

#endif