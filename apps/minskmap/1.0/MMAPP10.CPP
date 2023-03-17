#include <alloc.h>
#include <math.h>

#include "button.hpp"
#include "mmapp10.hpp"
#include "apputils.hpp"
#include "globals.hpp"
#include "mouse.hpp"
#include "funcs.hpp"
#include "menuall2.hpp"
#include "dbffunc.hpp"
#include "globalda.hpp"

/**************************
 * CMinskMap10Application *
 **************************/

CMinskMap10Application::CMinskMap10Application(int argc, char *argv[]) :
  CMinskMapApplication(argc, argv)
{
}

char const *CMinskMap10Application::getVersion(void) const
{
  return "MINSK_MAP 1.3";
}

void CMinskMap10Application::runMainMenu(void)
{
  setcolor(BLACK);
  mouse_show(OFF);
  line(480, 39, 480, 196);
  line(639, 39, 639, 196);
  button b1(480, 49, 559, 64, 1, "Все"),
      b2(560, 49, 639, 64, 2, "Фрагмент"),
      b3(480, 65, 559, 80, 3, "Центр"),
      b4(560, 65, 639, 80, 4, "Возврат"),
      b31(480, 81, 559, 96, 14, "Крупнее"),
      b41(560, 81, 639, 96, 15, "Мельче"),
      b5(480, 97, 559, 112, 5, "Расст-ние"),
      b51(560, 97, 639, 112, 16, ston),
      b6(480, 123, 559, 138, 6, "Что это"),
      b7(560, 123, 639, 138, 7, "Дом"),
      b8(480, 139, 559, 154, 8, "Улица"),
      b9(560, 139, 639, 154, 9, "Район"),
      b91(480, 155, 559, 170, 17, "Печать"),
      b92(560, 155, 639, 170, 18, "Слайд"),
      b10(480, 181, 559, 196, 10, "Вкл/Выкл"),
      b11(560, 181, 639, 196, 11, "Знак"),
      b12(480, 197, 559, 212, 12, "Что здесь"),
      b13(560, 197, 639, 212, 13, "Поиск"),
      b14(480, 213, 559, 228, 19, "Новая"),
      b15(560, 213, 639, 228, 20, "Удал.базу"),
      b16(480, 229, 559, 244, 21, "Назначить"),
      b17(560, 229, 639, 244, 22, "Ввод"),
      b18(480, 245, 559, 260, 23, "Изменение"),
      b19(560, 245, 639, 260, 24, "Уд.объект"),
      b20(480, 263, 559, 278, 25, "Справка"),
      b21(560, 263, 639, 278, -1, "Выход");
  btnOnOff = &b51;
  Menu_but menm;
  menm + &b1 + &b2 + &b3 + &b4 + &b31 + &b41 + &b5 + &b51 + &b6 + &b7 + &b8 + &b9 + &b91 + &b92 +
      &b10 + &b11 + &b12 + &b13 + &b14 + &b15 + &b16 + &b17 + &b18 + &b19 + &b20 + &b21;
  menm.draw();
  outtextxy(560, 44, "О б з о р");
  outtextxy(560, 118, "К а р т а");
  outtextxy(562, 176, "Б а з ы");
  mouse_show(ON);
  if (parst)
    if (find_bl(parst, parbl))
      all();

  runMenu(&menm);
}
