#ifndef __VIEWCTX_HPP__
#define __VIEWCTX_HPP__

#include "fastmod.hpp"
#include "geometry.hpp"

class CViewContext
{
private:
  CRectS *mapClipBounds;
  CRectD *mapTotalBounds;

  unsigned minObjectSize;
public:
  CViewContext(void);
  virtual ~CViewContext(void);

  int checkObjectVisible(F_SPR const& spr) const;
  CRectS const& getMapClipBounds(void) const;
  CRectD const& getMapTotalBounds(void) const;
  unsigned getMinObjectSize(void) const;
  void setMapClipBounds(int left, int top, int right, int bottom) const;
  void setMapTotalBounds(double left, double top, double right, double bottom) const;
  void setMinObjectSize(unsigned value);
};

inline CRectS const& CViewContext::getMapClipBounds(void) const
{
  return *mapClipBounds;
}

inline CRectD const& CViewContext::getMapTotalBounds(void) const
{
  return *mapTotalBounds;
}

inline unsigned CViewContext::getMinObjectSize(void) const
{
  return minObjectSize;
}


#endif
