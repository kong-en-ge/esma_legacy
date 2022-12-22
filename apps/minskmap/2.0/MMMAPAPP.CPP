#include <alloc.h>
#include <math.h>

#include "mmmapapp.hpp"
#include "apputils.hpp"
#include "globals.hpp"
#include "mouse.hpp"
#include "funcs.hpp"

const char *errorMessages[] =
{
  "Нет памяти для работы программы",                         // 0
  "Ошибка чтения карты",                                     // 1
  "Фатальные ошибки инициализации.  Продолжение невозможно", // 2
  "Ошибка записи слайда"                                     // 3
};

const char * warningMessages[] =
{
  "Не найден объект в указанной точке",         // 0
  "Достигнут максимальный масштаб отображения", // 1
  "Не найден заданный объект",                  // 2
  "Прямого транспорта нет",                     // 3
  "Маршрут не найден"                           // 4
};

void main_menu(char const* parst, int parbl);

/************************
 * CMinskMapApplication *
 ************************/

CMinskMapApplication::CMinskMapApplication(int argc, char *argv[]) :
  CInfoMapApplication(argc, argv)
{
  SCALEMIN = 400;
  SCALEMAX = 200000;
  this->argc = argc;
  this->argv = argv;
}

void CMinskMapApplication::executeApp(void)
{
  char *stpar = NULL;
  int blpar = 0;

  if (argc == 3)
  {
    stpar = argv[1];
    blpar = atoi(argv[2]);
    memmove(&XMINM, &XMING, 4 * sizeof(double));
  }
  else
    all();

  do
  {
    main_menu(stpar, blpar);
    stpar = NULL;
    blpar = 0;
  }
  while (!confirmExit());
}

char const *CMinskMapApplication::getIndexMapFilename(void) const
{
  return "MINSK.CWN";
}

char const *CMinskMapApplication::getMainMapFilename(void) const
{
  return "minsk";
}

char const *CMinskMapApplication::getVersion(void) const
{
  return "MINSK_MAP 2.0";
}


int CMinskMapApplication::runUI(void)
{
  setErrorMessages(errorMessages, sizeof(errorMessages) / sizeof(errorMessages[0]));
  setWarningMessages(warningMessages, sizeof(warningMessages) / sizeof(warningMessages[0]));
  LBSFilename = "MINSK.LBS";

  setfillstyle(1, BLACK);
  bar(0, 0, getmaxx(), getmaxy());

  int initState = initializeApplication();
  if (initState != STATE_SUCCESS)
  {
    displayError(state2error(initState));
    return 0;
  }

  if (argc != 3)
    if (!displaySplashBox(getVersion()))
      return 0;

  mouse_show(OFF);
  cleardevice();

  GRwnd* wndTitle = createTitleWindow();

  indexMapImage.paintImage(XMINEC, YMINEC);
  mouse_show(ON);

  executeApp();

  delete wndTitle;
  return 1;
}
