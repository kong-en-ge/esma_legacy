#include <alloc.h>
#include <math.h>

#include "rbmapapp.hpp"
#include "apputils.hpp"
#include "globals.hpp"
#include "mouse.hpp"
#include "funcs.hpp"
#include "menuall2.hpp"

/*********************
 * CRbMapApplication *
 *********************/

CRbMapApplication::CRbMapApplication(int argc, char *argv[]) :
  CInfoMapApplication(argc, argv)
{
  LBSFilename = "BELARUS.LBS";
  SCALEMIN = 50000;
  SCALEMAX = 10000000;
}

void CRbMapApplication::executeApp(void)
{
  all(getViewContext());

  do
  {
    runMainMenu();
  }
  while (!confirmExit());
}

char const *CRbMapApplication::getIndexMapFilename(void) const
{
  return "BELARUS.CWN";
}

char const *CRbMapApplication::getHelpFilename(void) const
{
  return "R_B.hlp";
}

char const *CRbMapApplication::getMainMapFilename(void) const
{
  return "belarus";
}

char const *CRbMapApplication::getVersion(void) const
{
  return "RB_MAP 1.0";
}

void CRbMapApplication::handleCommand(int command)
{
  switch (command)
  {
  case 8:
    find_punct(getViewContext());
    break;

  case 9:
    find_rg(getViewContext());
    break;

  default:
    CInfoMapApplication::handleCommand(command);
  }
}

void CRbMapApplication::runMainMenu(void)
{
  setcolor(BLACK);
  mouse_show(OFF);
  line(480, 39, 480, 180);
  line(639, 39, 639, 180);
  button b1(480, 49, 559, 64, 1, "Все"),
      b2(560, 49, 639, 64, 2, "Фрагмент"),
      b3(480, 65, 559, 80, 3, "Центр"),
      b4(560, 65, 639, 80, 4, "Возврат"),
      b31(480, 81, 559, 96, 14, "Крупнее"),
      b41(560, 81, 639, 96, 15, "Мельче"),
      b6(480, 107, 559, 122, 6, "Что это"),
      b7(560, 107, 639, 122, 5, "Расст-ние"),
      b8(480, 123, 559, 138, 8, "Нас.пункт"),
      b9(560, 123, 639, 138, 9, "Район"),
      b91(480, 139, 559, 154, 17, "Печать"),
      b92(560, 139, 639, 154, 18, "Слайд"),
      b10(480, 165, 559, 180, 10, "Вкл/Выкл"),
      b11(560, 165, 639, 180, 11, "Знак"),
      b12(480, 181, 559, 196, 12, "Что здесь"),
      b13(560, 181, 639, 196, 13, "Поиск"),
      b14(480, 197, 559, 212, 19, "Новая"),
      b15(560, 197, 639, 212, 20, "Удал.базу"),
      b16(480, 213, 559, 228, 21, "Назначить"),
      b17(560, 213, 639, 228, 22, "Ввод"),
      b18(480, 229, 559, 244, 23, "Изменение"),
      b19(560, 229, 639, 244, 24, "Уд.объект"),
      b20(480, 246, 559, 261, 25, "Справка"),
      b21(560, 246, 639, 261, -1, "Выход");
  Menu_but menm;
  menm + &b1 + &b2 + &b3 + &b4 + &b31 + &b41 + &b6 + &b7 + &b8 + &b9 + &b91 + &b92 + &b10 +
      &b11 + &b12 + &b13 + &b14 + &b15 + &b16 + &b17 + &b18 + &b19 + &b20 + &b21;
  menm.draw();
  outtextxy(560, 44, "О б з о р");
  outtextxy(560, 102, "К а р т а");
  outtextxy(562, 160, "Б а з ы");
  mouse_show(ON);
  runMenu(&menm);
}
