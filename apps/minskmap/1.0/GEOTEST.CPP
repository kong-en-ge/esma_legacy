#include <alloc.h>
#include <math.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>
#include <stdlib.h>

#include "button.hpp"
#include "globalda.hpp"
#include "mouse.hpp"
#include "textfile.hpp"
#include "apputils.hpp"

extern unsigned _stklen = 10240;
void main_menu(char *parst, int parbl);
void DBF_FIL();
char VERSION[] = "MINSK_MAP 1.3";
char modelname[] = "minsk";
char modelmod[] = "00";
void drawcram();
void all();

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
  if (!GdEGA)
  {
    static unsigned char arr[][4] = {
        {EGA_BLACK, 0, 0, 0},
        {EGA_BLUE, 0, 0, 0x2D},
        {EGA_GREEN, 0x2, 0x2A, 0x2},
        {EGA_RED, 0x2A, 2, 2},
        {EGA_BROWN, 0x2A, 0x15, 0},
        {EGA_LIGHTGRAY, 0x1D, 0x1D, 0x1D},
        {EGA_DARKGRAY, 0x4, 0x4, 0x4},
        {EGA_LIGHTBLUE, 0x15, 0x15, 0x3F},
        {EGA_LIGHTGREEN, 0x4, 0x3A, 0x4},
        {EGA_LIGHTCYAN, 0x20, 0x3F, 0x3F},
        {EGA_LIGHTRED, 0x3F, 0x15, 0},
        {EGA_LIGHTMAGENTA, 0x3F, 0x23, 0x16},
        {EGA_YELLOW, 0x3A, 0x3A, 0x13},
        {EGA_WHITE, 0x2A, 0x2A, 0x2A}};
    for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++)
      setrgbpalette(arr[i][0], arr[i][1], arr[i][2], arr[i][3]);
  }
  else
  {
    setpalette(0, 55);
    setpalette(1, 25);
    setpalette(2, 34);
    setpalette(3, 35);
    setpalette(4, 36);
    setpalette(6, 38);
  };
  setcolor(BLACK);
  if (farcoreleft() <= 0x18000l)
  {
    displayError(0);
    return -1;
  }
  DBF_FIL();
  SCALEO = (log(SCALEMAX / SCALEMIN) / 256);
  LBSsize = 0;
  LBS = new List_base[100];
  TEXTFILE txt;
  if (txt.open("MINSK.LBS", 4000) != -1)
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
  if (!GdEGA)
  {
    int hnd;
    if ((hnd = open("MINSK.CWN", O_BINARY)) == -1)
      return -1;
    if ((bram = new char[filelength(hnd)]) == NULL)
    {
      displayError(0);
      return -1;
    }
    read(hnd, bram, filelength(hnd));
    close(hnd);
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
  if (!GdEGA)
    putimage(XMINEC, YMINEC, bram, COPY_PUT);
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

  do
  {
    main_menu(stpar, blpar);
    stpar = NULL;
    blpar = 0;
  }
  while (!confirmExit());

  if (!GdEGA)
    delete (bram);
exit:
  closegraph();
  return 0;
}

