#include "grwnd.hpp"

GRwnd::GRwnd(int xl, int yt, int xr, int yb, PCSTR title_, int titlec_, 
  int rm, int drw) : 
  Sreg(xl - BORD_S, yt - BORD_S, xr + BORD_S, yb + BORD_S, rm),
  title(title_)	 
{
  titlec = titlec_;
  nst = 1;
  if (drw)
    draw();
}
