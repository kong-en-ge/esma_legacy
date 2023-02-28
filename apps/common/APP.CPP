#include <alloc.h>
#include <string.h>
#include <dir.h>

#include "app.hpp"
#include "utils.hpp"

extern unsigned _stklen = 10240;

CApplication::CApplication(int argc, char *argv[])
{
  char szDir[MAXPATH];

  getcurdir(0, szDir);
  pSaveDir = (char*)malloc(strlen(szDir) + 1);
  strcpy(pSaveDir, szDir);

  if (findCommandLineParam(argc, argv, "workpath", szDir))
    chdir(szDir);
}

CApplication::~CApplication(void)
{
  if (pSaveDir)
  {
    chdir("\\");
    chdir(pSaveDir);
    free(pSaveDir);
  }
}
