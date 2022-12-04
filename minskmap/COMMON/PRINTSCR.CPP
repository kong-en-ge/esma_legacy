#include "button.hpp"
#include "menuall.hpp"
#include "menuall2.hpp"

int far pascal error2but()
{
  GRwnd wnd(170, 100, 450, 180, "Ошибка!", LIGHTRED, 1, 1);
  button b1(222, 155, 293, 171, 1, "Да"),
      b2(332, 155, 403, 171, -1, "Выход");
  wnd.text("");
  wnd.text("Принтер не готов! Продолжить?");
  Menu_but menu;
  menu + &b1 + &b2;
  return menu.menu_key();
}

int far pascal warn2but()
{
  int x, y;
  if (mouse_key_status(&x, &y) != -1)
    return 0;
  GRwnd wnd(170, 100, 450, 180, "Внимание!", LIGHTBLUE, 1, 1);
  button b1(212, 155, 303, 171, 1, "Да"),
      b2(322, 155, 413, 171, -1, "Продолжить");
  wnd.text("");
  wnd.text("Вы хотите прервать печать?");
  Menu_but menu;
  menu + &b1 + &b2;
  return menu.menu_key() == -1 ? 0 : 1;
}

struct PRINTPARAM
{
  int left, top, right, bottom;
  void *errorfunc;
  void *getpixelfunc;
  void *retfunc;
};

int printwnd(int xmin, int ymin, int xmax, int ymax, int /*mask1*/, int /*mask2*/)
{
  int h, size;
  if ((h = open("printer.bin", O_BINARY | O_RDONLY)) == -1)
    return -1;
  size = filelength(h);
  char *buf = new char[size + 16];
  char *proc = buf;
  (long)proc &= 0xfffffff0;
  (long)proc += 0x10000;
  if (read(h, proc, size - 32) == -1)
    return -1;
  PRINTPARAM pr;
  pr.left = xmin;
  pr.top = ymin;
  pr.right = xmax;
  pr.bottom = ymax;
  pr.errorfunc = error2but;
  pr.getpixelfunc = getpixel;
  pr.retfunc = warn2but;
  ((void pascal (*)(void *))proc)(&pr);
  close(h);
  delete buf;
  return 0;
}
