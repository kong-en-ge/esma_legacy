#ifndef __MAPHAND_HPP__
#define __MAPHAND_HPP__

#include "viewctx.hpp"

class CMapDataHandler
{
public:
  virtual ~CMapDataHandler(void);

  bool drawMap(CViewContext const& viewContext);
  virtual bool draw_one_obj(CViewContext const& viewContext, unsigned obj, 
    int color, int drawDisabled = 0) = 0;
};

#endif
