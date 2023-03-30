#include "maphand.hpp"
#include "globals.hpp"
#include "mouse.hpp"

CMapDataHandler::~CMapDataHandler(void)
{
}

int CMapDataHandler::drawMap(CViewContext const& viewContext)
{
  int exp = 0;
  F_SPR spr;
  for (unsigned i = model.pm.start; i <= model.pm.end; i++)
  {
    int key;
#ifndef DEMO
    if ((mouse_key_status(&key, &key)) == -1)
    {
      while (mouse_key_status(&key, &key))
        ;
      exp = -1;
      break;
    }
#endif
    draw_one_obj(viewContext, i, -1, 0 /* drawDisabled */);
  }
drawex:
  return exp;
}