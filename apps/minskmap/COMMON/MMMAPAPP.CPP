#include "mmmapapp.hpp"
#include "globals.hpp"
#include "funcs.hpp"
#include "apputils.hpp"
#include "globalda.hpp"
#include "menuall2.hpp"

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

void CMinskMapApplication::executeApp(void)
{
  parst = NULL;
  parbl = 0;

  if (argc == 3)
  {
    parst = argv[1];
    parbl = atoi(argv[2]);
    boundsM = boundsG;
  }
  else
    all();

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
    if (N_on)
    {
      N_on = 0;
      btnOnOff->text = ston;
    }
    else
    {
      N_on = 1;
      btnOnOff->text = stoff;
    }
    getActiveMenu()->draw();
    if (CURRENTSCALE < 12.0 * SCALEMIN)
      drawrazd();
    break;
  case 7:
    find_bl();
    break;
  case 8:
    find_st();
    break;
  case 9:
    find_rg();
    break;
  default:
    CInfoMapApplication::handleCommand(command);
  }
}

int CMinskMapApplication::suppressStartSplash(void) const
{
  return (argc == 3);
}
