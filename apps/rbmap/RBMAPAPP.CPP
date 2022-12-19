#include <alloc.h>
#include <math.h>

#include "rbmapapp.hpp"
#include "apputils.hpp"
#include "globals.hpp"
#include "mouse.hpp"
#include "funcs.hpp"

#define STATE_SUCCESS 0
#define STATE_LOW_MEMORY 1
#define STATE_NO_MAP 2
#define STATE_FAILED 3
#define STATE_CANCELLED 4

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
  "Не найден заданный объект"                   // 2
};

void main_menu();
void DBF_FIL();
char VERSION[] = "RB_MAP 1.0";
char modelname[] = "belarus";
char modelmod[] = "00";

int state2error(int state)
{
  switch(state)
  {
    case STATE_FAILED:
      return 2;

    case STATE_LOW_MEMORY:
      return 0;

    case STATE_NO_MAP:
      return 1;
  }

  return -1;
}

/*********************
 * CRbMapApplication *
 *********************/

CRbMapApplication::CRbMapApplication(int argc, char *argv[]) :
  CGraphicApplication(argc, argv)
{
  SCALEMIN = 50000;
  SCALEMAX = 10000000;
}

void CRbMapApplication::executeApp(void)
{
  all();

  do
  {
    main_menu();
  }
  while (!confirmExit());
}

int CRbMapApplication::initializeApplication(void)
{
  if (farcoreleft() <= 0x18000l)
    return STATE_LOW_MEMORY;

  YMAXMAPE = getmaxy();

  if (!loadIndexMap())
    return STATE_FAILED;

  init_mouse();
  mouse_screen_size(XMAXMAPE, 0, getmaxx(), getmaxy());
  mouse_show(ON);


  DBF_FIL();
  loadLibDefs();
  SCALEO = (log(SCALEMAX / SCALEMIN) / 256);

  if (!openMapData())
    return STATE_NO_MAP;


  if (displaySplashBox(VERSION))
    return STATE_SUCCESS;
  else
    return STATE_CANCELLED;
}

int CRbMapApplication::loadIndexMap(void)
{
  if (!isModeEGA())
  {
    if (!indexMapImage.load("BELARUS.CWN"))
      return 0;

    XMINEC = 481;
    YMINEC = getmaxy() - 158;
    XMAXEC = 638;
    YMAXEC = getmaxy() - 1;
  }

  return 1;
}

int CRbMapApplication::openMapData(void)
{
  if (model.open(modelname, modelmod) == 0)
  {
    double dxv, dyv;

    memmove(&XMING, &model.fm.xminm, 4 * sizeof(XMING));
    memmove(&XMINV, &XMING, 4 * sizeof(XMING));
    dyv = XMAXV - XMINV;
    dxv = YMAXV - YMINV;
    if (dxv > dyv)
      dyv = dxv;
    else
      dxv = dyv;
    XMINVC = (XMAXV + XMINV - dxv) / 2;
    XMAXVC = XMINVC + dxv;
    YMINVC = (YMAXV + YMINV - dyv) / 2;
    YMAXVC = YMINVC + dyv;

    return 1;
  }
  else
    return 0;
}

int CRbMapApplication::runUI(void)
{
  setErrorMessages(errorMessages, sizeof(errorMessages) / sizeof(errorMessages[0]));
  setWarningMessages(warningMessages, sizeof(warningMessages) / sizeof(warningMessages[0]));
  LBSFilename = "BELARUS.LBS";

  setfillstyle(1, BLACK);
  bar(0, 0, getmaxx(), getmaxy());

  int initState = initializeApplication();
  if (initState != STATE_SUCCESS)
  {
    if (initState != STATE_CANCELLED)
      displayError(state2error(initState));

    return 0;
  }

  mouse_show(OFF);
  cleardevice();

  GRwnd wndmain2(487, 8, 632, 31, "ООО 'ЭСМА'", LIGHTBLUE, 0, 0);
  wndmain2.draw();
  wndmain2.text(VERSION);

  indexMapImage.paintImage(XMINEC, YMINEC);
  mouse_show(ON);

  executeApp();

  return 1;
}

