#include <alloc.h>
#include <math.h>

#include "button.hpp"
#include "mmapp20.hpp"
#include "apputils.hpp"
#include "globals.hpp"
#include "mouse.hpp"
#include "funcs.hpp"
#include "menuall2.hpp"
#include "dbffunc.hpp"
#include "globalda.hpp"

const char * warningMessages[] =
{
  "Не найден объект в указанной точке",         // 0
  "Достигнут максимальный масштаб отображения", // 1
  "Не найден заданный объект",                  // 2
  "Прямого транспорта нет",                     // 3
  "Маршрут не найден"                           // 4
};

/**************************
 * CMinskMap20Application *
 **************************/

CMinskMap20Application::CMinskMap20Application(int argc, char *argv[]) :
  CMinskMapApplication(argc, argv)
{
}

char const *CMinskMap20Application::getVersion(void) const
{
  return "MINSK_MAP 2.0";
}

char const ** CMinskMap20Application::getWarningMessages(void) const
{
  return warningMessages;
}

int CMinskMap20Application::getWarningMessageCount(void) const
{
  return sizeof(warningMessages) / sizeof(warningMessages[0]);
}

void CMinskMap20Application::handleCommand(int command)
{
  switch (command)
  {
  case 26:
    howstop(getViewContext());
    break;

  case 27:
    where_from(getViewContext());
    break;

  case 28:
    if (pr)
      where(getViewContext());
    break;

  default:
    CMinskMapApplication::handleCommand(command);
  }
}

void CMinskMap20Application::runMainMenu(void)
{
  prmt = 0;
  uzl = 0;
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
      b81(480, 155, 533, 170, 26, "Трансп"),
      b82(534, 155, 587, 170, 27, "Откуда"),
      b83(588, 155, 639, 170, 28, "Куда"),
      b91(480, 171, 559, 186, 17, "Печать"),
      b92(560, 171, 639, 186, 18, "Слайд"),
      b10(480, 197, 559, 212, 10, "Вкл/Выкл"),
      b11(560, 197, 639, 212, 11, "Знак"),
      b12(480, 213, 559, 228, 12, "Что здесь"),
      b13(560, 213, 639, 228, 13, "Поиск"),
      b14(480, 229, 559, 244, 19, "Новая"),
      b15(560, 229, 639, 244, 20, "Удал.базу"),
      b16(480, 245, 559, 260, 21, "Назначить"),
      b17(560, 245, 639, 260, 22, "Ввод"),
      b18(480, 261, 559, 276, 23, "Изменение"),
      b19(560, 261, 639, 276, 24, "Уд.объект"),
      b20(480, 279, 559, 294, 25, "Справка"),
      b21(560, 279, 639, 294, -1, "Выход");
  btnOnOff = &b51;
  Menu_but menm;
  menm + &b1 + &b2 + &b3 + &b4 + &b31 + &b41 + &b5 + &b51 + &b6 + &b7 + &b8 + &b9 + &b81 + &b82 + &b83 + &b91 + &b92 +
      &b10 + &b11 + &b12 + &b13 + &b14 + &b15 + &b16 + &b17 + &b18 + &b19 + &b20 + &b21;
  menm.draw();
  outtextxy(560, 44, "О б з о р");
  outtextxy(560, 118, "К а р т а");
  outtextxy(562, 191, "Б а з ы");
  mouse_show(ON);
  if (parst)
    if (find_bl(getViewContext(), parst, parbl))
      all(getViewContext());
  runMenu(&menm);
  if (prmt == 1)
    delete metro;
  if (uzl == 1)
    delete uzly;
}
