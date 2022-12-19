#include <iostream.h>

#include "graphapp.hpp"
#include "globals.hpp"

CGraphicApplication::CGraphicApplication(int argc, char *argv[]) :
  CApplication(argc, argv)
{
}

int CGraphicApplication::enterGraphicMode(void)
{
  setgraphbufsize(8000);
  int gd, gm;
  detectgraph(&gd, &gm);
  if ((gd != EGA) && (gd != VGA))
    gd = VGA;
  _graphicMode = gd;
  GdEGA = gd == EGA;

  initgraph(&gd, &gm, NULL);
  if (graphresult() == grOk)
  {
    setupPalette();
    return 1;
  }
  else
    return 0;
}

void CGraphicApplication::leaveGraphicMode(void)
{
  closegraph();
}

int CGraphicApplication::run(void)
{
  int retValue;

  if (enterGraphicMode())
  {
    retValue = runUI();

    leaveGraphicMode();
  }
  else
  {
    cout << "Ошибка инициализации графического режима" << "\n";
    retValue = EXIT_FAILURE;
  }

  return retValue;
}

void CGraphicApplication::setupPalette(void)
{
  if (!isModeEGA())
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
    setpalette(1, 25);
    setpalette(2, 34);
    setpalette(3, 35);
    setpalette(4, 36);
    setpalette(6, 38);

  }
  setpalette(15, EGA_BLACK);
  setpalette(0, EGA_WHITE);
}


