#ifndef txtpacks
#define txtpacks
#include "textpack.hpp"
#include "tpedit.hpp"

class TEXT_PACK_MENUS : public TEXT_PACK_MENU
{
public:
  TEXT_PACK_MENUS(int xl, int yt, int xr, int sizep_, char *titl,
                  char *first, int rem, int maxsize, int maxel, int fstr = 1, int estr = 0) : TEXT_PACK_MENU(xl, yt, xr, sizep_, titl, first, rem, maxsize, maxel, fstr, estr){};
  void draw_one(int num)
  {
    SYNC = 1;
    TEXT_PACK_MENU::draw_one(num);
    SYNC = 0;
  }
};
#endif
