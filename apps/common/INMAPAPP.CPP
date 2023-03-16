#include <alloc.h>
#include <math.h>
#include <string.h>

#include "inmapapp.hpp"
#include "globals.hpp"
#include "mouse.hpp"
#include "funcs.hpp"
#include "apputils.hpp"
#include "textfile.hpp"

static const char *errorMessages[] =
{
  "Нет памяти для работы программы",                         // 0
  "Ошибка чтения карты",                                     // 1
  "Фатальные ошибки инициализации.  Продолжение невозможно", // 2
  "Ошибка записи слайда"                                     // 3
};

static const char * warningMessages[] =
{
  "Не найден объект в указанной точке",         // 0
  "Достигнут максимальный масштаб отображения", // 1
  "Не найден заданный объект"                   // 2
};

void DBF_FIL();

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

/***********************
 * CInfoMapApplication *
************************/

CInfoMapApplication::CInfoMapApplication(int argc, char *argv[]) :
  CGraphicApplication(argc, argv)
{
}

void CInfoMapApplication::displayHelp(void) const
{
  help(getHelpFilename());
}

char const ** CInfoMapApplication::getErrorMessages(void) const
{
  return errorMessages;
}

int CInfoMapApplication::getErrorMessageCount(void) const
{
  return sizeof(errorMessages) / sizeof(errorMessages[0]);
}

char const ** CInfoMapApplication::getWarningMessages(void) const
{
  return warningMessages;
}

int CInfoMapApplication::getWarningMessageCount(void) const
{
  return sizeof(warningMessages) / sizeof(warningMessages[0]);
}

int CInfoMapApplication::initializeApplication(void)
{
  if (farcoreleft() <= 0x18000l)
    return STATE_LOW_MEMORY;

  setErrorMessages(getErrorMessages(), getErrorMessageCount());
  setWarningMessages(getWarningMessages(), getWarningMessageCount());

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


  return STATE_SUCCESS;
}

int CInfoMapApplication::loadIndexMap(void)
{
  if (!isModeEGA())
  {
    if (!indexMapImage.load(getIndexMapFilename()))
      return 0;

    XMINEC = 481;
    YMINEC = getmaxy() - 158;
    XMAXEC = 638;
    YMAXEC = getmaxy() - 1;
  }

  return 1;
}

void CInfoMapApplication::loadLibDefs(void)
{
  LBSsize = 0;
  LBS = new List_base[100];
  TEXTFILE txt;
  if (txt.open(LBSFilename, 4000) != -1)
  {
    char str[41];
    while (LBSsize < 100)
    {
      if (txt.readstr(str, 40) < 0)
        break;
      char *c = strchr(str, '/');
      if (!c)
        continue;
      *c++ = 0;
      strcpy((LBS + LBSsize)->name, str);
      char *c1 = strchr(c, '/');
      if (!c1)
        continue;
      *c1++ = 0;
      strcpy((LBS + LBSsize)->file, c);
      memmove((LBS + LBSsize)->mark, c1, 2);
      (LBS + LBSsize)->activ = 0;
      LBSsize++;
    }
  }
}

int CInfoMapApplication::openMapData(void)
{
  if (model.open(getMainMapFilename(), "00") == 0)
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

int CInfoMapApplication::runUI(void)
{
  setfillstyle(1, BLACK);
  bar(0, 0, getmaxx(), getmaxy());

  int initState = initializeApplication();
  if (initState != STATE_SUCCESS)
  {
    displayError(state2error(initState));
    return EXIT_FAILURE;
  }

  if (!suppressStartSplash())
    if (!displaySplashBox(getVersion()))
      return EXIT_SUCCESS;

  mouse_show(OFF);
  cleardevice();

  GRwnd* wndTitle = createTitleWindow();

  indexMapImage.paintImage(XMINEC, YMINEC);
  mouse_show(ON);

  executeApp();

  delete wndTitle;
  return EXIT_SUCCESS;
}

int CInfoMapApplication::suppressStartSplash(void) const
{
  return 0;
}
