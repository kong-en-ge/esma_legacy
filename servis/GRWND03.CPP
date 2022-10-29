#include "grwnd.hpp"

pascal GRwnd::GRwnd(int xl,int yt,int xr,int yb,char *title_,int titlec_,int rm,int drw)
 : Sreg(xl-BORD_S,yt-BORD_S,xr+BORD_S,yb+BORD_S,rm) {
 titlec=titlec_;
 nst=1;
 title=title_;
 if (drw) draw();
}

