#ifndef __RBMAPHN_HPP__
#define __RBMAPHN_HPP__

#include "maphand.hpp"

class CRbMapDataHandler : public CMapDataHandler
{
public:
  virtual bool draw_one_obj(CViewContext const& viewContext, unsigned obj,
    int color, int drawDisabled = 0);
};

#endif