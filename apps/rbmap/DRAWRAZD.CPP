#include "globals.hpp"

void drawcram(void)
{
  if (!indexMapImage.hasData())
    return;

  indexMapImage.paintImage(XMINEC, YMINEC);

  int xminec, yminec, xmaxec, ymaxec;
  xminec = XMINEC + (boundsV.top - YMINVC) / (YMAXVC - YMINVC) * (XMAXEC - XMINEC);
  yminec = YMAXEC - (boundsV.right - XMINVC) / (XMAXVC - XMINVC) * (YMAXEC - YMINEC);
  xmaxec = XMINEC + (boundsV.bottom - YMINVC) / (YMAXVC - YMINVC) * (XMAXEC - XMINEC);
  ymaxec = YMAXEC - (boundsV.left - XMINVC) / (XMAXVC - XMINVC) * (YMAXEC - YMINEC);
  int d = -1;
  // if ((xmaxec-xminec)<64) d=0;
  if ((xmaxec - xminec) <= 16)
    d = (16 - (xmaxec - xminec)) / 2;
  setcolor(BLACK);
  setlinestyle(0, 0, 3);
  rectangle(xminec - d, yminec - d, xmaxec + d, ymaxec + d);
  setlinestyle(0, 0, 1);
}
