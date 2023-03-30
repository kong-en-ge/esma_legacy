#include "mmmapapp.hpp"
#include "globals.hpp"
#include "funcs.hpp"
#include "apputils.hpp"
#include "menuall2.hpp"
#include "mmmaphn.hpp"

char const * CMinskMapApplication::ston = "Вкл. N";
char const * CMinskMapApplication::stoff = "Выкл. N";

CMinskMapApplication::CMinskMapApplication(int argc, char *argv[]) :
  CInfoMapApplication(argc, argv)
{
  LBSFilename = "MINSK.LBS";
  SCALEMIN = 400;
  SCALEMAX = 200000;
  this->argc = argc;
  this->argv = argv;
}

CMapDataHandler* CMinskMapApplication::createMapDataHandler(void)
{
  pMapDataHandler = new CMinskMapDataHandler();
  return pMapDataHandler;
}

void CMinskMapApplication::executeApp(void)
{
  parst = NULL;
  parbl = 0;

  if (argc == 3)
  {
    parst = argv[1];
    parbl = atoi(argv[2]);
    boundsM = getViewContext().getMapTotalBounds();
  }
  else
    all(getViewContext());

  do
  {
    runMainMenu();
    parst = NULL;
    parbl = 0;
  }
  while (!confirmExit());
}

char const *CMinskMapApplication::getIndexMapFilename(void) const
{
  return "MINSK.CWN";
}

char const *CMinskMapApplication::getHelpFilename(void) const
{
  return "M_M.hlp";
}

char const *CMinskMapApplication::getMainMapFilename(void) const
{
  return "minsk";
}

void CMinskMapApplication::handleCommand(int command)
{
  switch (command)
  {
  case 16:
    if (pMapDataHandler->getDisplayNumbers())
    {
      pMapDataHandler->setDisplayNumbers(false);
      btnOnOff->text = ston;
    }
    else
    {
      pMapDataHandler->setDisplayNumbers(true);
      btnOnOff->text = stoff;
    }
    getActiveMenu()->draw();
    if (CURRENTSCALE < 12.0 * SCALEMIN)
      drawrazd(getViewContext());
    break;

  case 7:
    find_bl(getViewContext());
    break;

  case 8:
    find_st(getViewContext());
    break;

  case 9:
    find_rg(getViewContext());
    break;

  default:
    CInfoMapApplication::handleCommand(command);
  }
}

bool CMinskMapApplication::suppressStartSplash(void) const
{
  return (argc == 3);
}
