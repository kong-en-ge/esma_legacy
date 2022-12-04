#include <dos.h>
#include <stdlib.h>
#include "grwnd.hpp"

int GRwnd::readfloat(float *fl, float minfl, float maxfl, int MAXL, char *f, int x_posit)
{
  double d = *fl;
  if (readdouble(&d, minfl, maxfl, MAXL, f, x_posit))
    return 1;
  *fl = d;
  return 0;
}

int GRwnd::readdouble(double *fl, double minfl, double maxfl, int MAXL, char *f, int x_posit)
{
  char numb[20], *es;
  double fl1;
  gcvt(*fl, MAXL, numb);
  do
  {
    if (readst(numb, MAXL, f, 0, BLACK | (LIGHTGRAY << 4), x_posit))
      return 1;
    fl1 = strtod(numb, (char **)&es);
    if ((fl1 >= minfl) && (fl1 <= maxfl) && (*es == 0))
      break;
    sound(200);
    delay(50);
    nosound();
    nst--;
  } while (1);
  *fl = fl1;
  return 0;
}