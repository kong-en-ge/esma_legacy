#include <string.h>

#include "coretype.hpp"

bool findCommandLineParam(int argc, char *argv[], char const* param, char* value = NULL)
{
  char const* arg;
  int paramLen = strlen(param);

  for (int i = 1; i < argc; i++)
  {
    arg = argv[i];

    if (*arg != '/' && *arg != '-')
      continue;

    if (strncmpi(arg + 1, param, paramLen))
      continue;

    if (arg[paramLen + 1] != 0)
    {
      if (arg[paramLen + 1] != ':')
        continue;

      if (value != NULL)
        strcpy(value, arg + paramLen + 2);
    }

    return true;
  }

  return false;
}
