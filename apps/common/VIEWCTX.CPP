#include "graphics.h"
#include "viewctx.hpp"

CViewContext::CViewContext(void)
{
  mapClipBounds = new CRectS();
  mapViewBounds = new CRectS();
  mapTotalBounds = new CRectD();
}

CViewContext::~CViewContext(void)
{
  delete mapClipBounds;
  delete mapViewBounds;
  delete mapTotalBounds;
}

int CViewContext::checkObjectVisible(F_SPR const& spr) const
{
  return !(spr.xmax <= mapClipBounds->left || spr.xmin >= mapClipBounds->right ||
    spr.ymax <= mapClipBounds->top || spr.ymin >= mapClipBounds->bottom);
}

void CViewContext::setMapClipBounds(int left, int top, int right, int bottom) const
{
  mapClipBounds->setValues(left, top, right,bottom);
}

void CViewContext::setMapTotalBounds(double left, double top, double right, double bottom) const
{
  mapTotalBounds->setValues(left, top, right,bottom);
}

void CViewContext::setMapViewBounds(CRectS const& value) const
{
  *mapViewBounds = value;
}

void CViewContext::setMinObjectSize(unsigned value)
{
  minObjectSize = value;
}

void CViewContext::updateViewPort(void) const
{
  setviewport(mapViewBounds->left, mapViewBounds->top, mapViewBounds->right, mapViewBounds->bottom, 1);
}
