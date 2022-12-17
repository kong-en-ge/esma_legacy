#include <alloc.h>
#include <string.h>
#include <math.h>
#include <io.h>
#include <fcntl.h>
#include <stdlib.h>

#include "fstmod.hpp"
#include "drawpl.hpp"
#include "button.hpp"
#include "globalda.hpp"
#include "menuall2.hpp"
#include "mouse.hpp"
#include "flibrt.hpp"
#include "textfile.hpp"
#include "atribut.hpp"
#include "textpack.hpp"
#include "set.hpp"
#include "tpedit.hpp"
#include "apputils.hpp"
#include "funcs.hpp"

extern unsigned _stklen = 10240;
void main_menu(char *parst, int parbl);
void DBF_FIL();
char VERSION[] = "MINSK_MAP 2.0";
char modelname[] = "minsk";
char modelmod[] = "00";

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

int Init(int carg)
{
  setgraphbufsize(8000);
  int gd, gm;
  detectgraph(&gd, &gm);
  if ((gd != EGA) && (gd != VGA))
    gd = VGA;
  if (gd == EGA)
    GdEGA++;
  initgraph(&gd, &gm, NULL);
  gm = graphresult();
  if (gm != grOk)
    return -1;
  YMAXMAPE = getmaxy();
  setpalette(15, 0);
  setpalette(0, 63);
  setfillstyle(1, BLACK);
  bar(0, 0, getmaxx(), getmaxy());
  setupPalette(GdEGA);
  setcolor(BLACK);
  if (farcoreleft() <= 0x18000l)
  {
    displayError(0);
    return -1;
  }
  DBF_FIL();
  SCALEMIN = 400;
  SCALEMAX = 200000;
  SCALEO = (log(SCALEMAX / SCALEMIN) / 256);
  loadLibDefs();
  if (!GdEGA)
  {
    if (!indexMapImage.load("MINSK.CWN"))
      return 0;

    XMINEC = 481;
    YMINEC = getmaxy() - 158;
    XMAXEC = 638;
    YMAXEC = getmaxy() - 1;
  }
  init_mouse();
  mouse_screen_size(XMAXMAPE, 0, getmaxx(), getmaxy());
  mouse_show(ON);
  if (model.open(modelname, modelmod) == -1)
    return 1;
  if (carg == 3)
    return 0;

  return !displaySplashBox(VERSION);
}

int org_main(int argc, char *argv[])
{
  setErrorMessages(errorMessages, sizeof(errorMessages) / sizeof(errorMessages[0]));
  setWarningMessages(warningMessages, sizeof(warningMessages) / sizeof(warningMessages[0]));
  LBSFilename = "MINSK.LBS";

  GRwnd wndmain2(487, 8, 632, 31, "ООО 'ЭСМА'", LIGHTBLUE, 0, 0);
  int ii;
  double dxv, dyv;
  char *stpar = NULL;
  int blpar = 0;
  if ((ii = Init(argc)) != 0)
  {
    if (ii == -1)
      displayError(2);
    goto exit;
  }
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
  mouse_show(OFF);
  cleardevice();
  mouse_show(ON);
  wndmain2.draw();
  wndmain2.text(VERSION);
  mouse_show(OFF);
  indexMapImage.paintImage(XMINEC, YMINEC);
  mouse_show(ON);
  //     all();
  if (argc == 3)
  {
    stpar = argv[1];
    blpar = atoi(argv[2]);
    memmove(&XMINM, &XMING, 4 * sizeof(double));
  }
  else
    all();
label0:
  do
  {
    main_menu(stpar, blpar);

    stpar = NULL;
    blpar = 0;
  }
  while (!confirmExit());

//      delete metro;delete uzly;
exit:
  closegraph();
  return 0;
}
