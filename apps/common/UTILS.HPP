#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include <stdlib.h>

#include "coretype.hpp"

bool findCommandLineParam(int argc, char *argv[], char const* param, char* value = NULL);

#endif