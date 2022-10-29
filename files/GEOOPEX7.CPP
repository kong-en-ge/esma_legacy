#include <dir.h>
#include <dos.h>
#include <stdlib.h>
#include <string.h>
#include "geomodel.hpp"
int pascal GEO_MODEL::readdoublehars(SPR *spr,char *attr,double *d) {
 char str[20],*s;int ret;
 if ((ret=readhars(spr,attr,str,19))!=0) return ret;
 *d=strtod(str,&s);
 return 0;
}
