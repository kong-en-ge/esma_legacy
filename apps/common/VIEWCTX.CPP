#include "viewctx.hpp"

CViewContext::CViewContext(void)
{
  mapClipBounds = new CRectS();
}

CViewContext::~CViewContext(void)
{
  delete mapClipBounds;
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

void CViewContext::setMinObjectSize(unsigned value)
{
  minObjectSize = value;
}