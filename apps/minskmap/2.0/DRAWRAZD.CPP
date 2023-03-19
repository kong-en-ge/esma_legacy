#include "globalda.hpp"
#include "funcs.hpp"

void drawcram()
{
  if (!indexMapImage.hasData())
    return;

  indexMapImage.paintImage(XMINEC, YMINEC);

  int xminec, yminec, xmaxec, ymaxec;
  xminec = XMINEC + (YMINV - YMINVC) / (YMAXVC - YMINVC) * (XMAXEC - XMINEC);
  yminec = YMAXEC - (XMAXV - XMINVC) / (XMAXVC - XMINVC) * (YMAXEC - YMINEC);
  xmaxec = XMINEC + (YMAXV - YMINVC) / (YMAXVC - YMINVC) * (XMAXEC - XMINEC);
  ymaxec = YMAXEC - (XMINV - XMINVC) / (XMAXVC - XMINVC) * (YMAXEC - YMINEC);
  int d = -1;
  if ((xmaxec - xminec) < 64)
    d = 0;
  if ((xmaxec - xminec) <= 16)
    d = (16 - (xmaxec - xminec)) / 2;
  setlinestyle(0, 0, 3);
  setcolor(BLACK);
  rectangle(xminec - d, yminec - d, xmaxec + d, ymaxec + d);
  if (pr == 1)
  {
    int xy, yx;
    double xd, yd;
    xd = XDISRAZ(yo);
    yd = YDISRAZ(xo);
    xy = XMINEC + (yd - YMINVC) * (XMAXEC - XMINEC) / (YMAXVC - YMINVC);
    yx = YMINEC + (XMAXVC - xd) * (YMAXEC - YMINEC) / (XMAXVC - XMINVC);
    //   setcolor(GREEN);
    line(xy - 2, yx, xy + 2, yx);
    line(xy, yx - 2, xy, yx + 2);
  }
  setlinestyle(0, 0, 1);
}
