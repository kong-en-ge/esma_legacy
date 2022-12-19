#include "button.hpp"
#include "apputils.hpp"

int displaySplashBox(const char * version)
{
  GRwnd wnd2(170, 40, 470, 180, version, LIGHTBLUE, 0, 1);
  Menu_but mn;
  button b1(220, 155, 310, 171, 0, "Продолжить"),
         b2(330, 155, 420, 171, 1, "Выход");
  mn + &b1 + &b2;
  wnd2.text("");
  wnd2.text("Программа разработана ООО 'ЭСМА'");
  wnd2.text("Республика   Беларусь   г. Минск");
  wnd2.text("  тел.  (0172) - 20-96-36     ");
  wnd2.text("  факс. (0172) - 20-91-25 ESMA");
  wnd2.text("        (0172) - 27-00-13 ESMA");

  return mn.menu_key() == 0;
}

int confirmExit()
{
  GRwnd wnd(170, 100, 450, 180, "Внимание", LIGHTBLUE, 1, 1);
  button bt1(222, 155, 293, 171, 1, "Да"),
         bt2(332, 155, 403, 171, -1, "Heт");
  wnd.text("");
  wnd.text("Вы действительно хотите выйти?");
  Menu_but menu;
  menu + &bt2 + &bt1;

  return menu.menu_key() != -1;
}
