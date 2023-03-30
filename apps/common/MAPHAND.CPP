#include "maphand.hpp"
#include "globals.hpp"
#include "mouse.hpp"

CMapDataHandler::~CMapDataHandler(void)
{
}

bool CMapDataHandler::drawMap(CViewContext const& viewContext)
{
  bool retValue = true;
  F_SPR spr;
  for (unsigned i = model.pm.start; i <= model.pm.end; i++)
  {
#ifndef DEMO
    int key;
    if ((mouse_key_status(&key, &key)) == -1)
    {
      while (mouse_key_status(&key, &key))
        ;
      retValue = false;
      break;
    }
#endif
    draw_one_obj(viewContext, i, -1, 0 /* drawDisabled */);
  }

  return retValue;
}