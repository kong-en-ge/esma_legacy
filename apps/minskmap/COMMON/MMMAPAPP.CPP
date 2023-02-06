#include "mmmapapp.hpp"
#include "globals.hpp"
#include "funcs.hpp"
#include "apputils.hpp"

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
    memmove(&XMINM, &XMING, 4 * sizeof(double));
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
