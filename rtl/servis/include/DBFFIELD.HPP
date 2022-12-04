#ifndef DBFFIELDHPP
#define DBFFIELDHPP
#include "button.hpp"
#include "dbf.hpp"

struct Big_DBF : public Big_Menu
{
  int name1;
  DBF *d;

  Big_DBF(DBF *_d, int xl, int yt, int xr, int sizep, char *title, char *f_str,
          int rm, int first_str = 1, int end_str = 0);
  void draw_one(int);
  int menu_key(int field);
  int menu_key(char *filed_name);
  int menu_key(int filed1, char *f1_value, int field2, char *f2_value);
};

inline Big_DBF::Big_DBF(DBF *_d, int xl, int yt, int xr, int sizep, char *title, char *f_str,
                        int rm, int first_str, int end_str) : Big_Menu(xl, yt, xr, sizep, title, f_str, rm, first_str, end_str)
{
  d = _d;
}

#endif