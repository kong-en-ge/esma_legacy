#include <stdlib.h>
#include "geomodel.hpp"

int GEO_MODEL::readdoublehars(SPR *spr, char *attr, double *d)
{
  char str[20], *s;
  int ret;
  if ((ret = readhars(spr, attr, str, 19)) != 0)
    return ret;
  *d = strtod(str, &s);
  return 0;
}
