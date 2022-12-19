#include <alloc.h>
#include <string.h>
#include <dir.h>

#include "app.hpp"

static const char cWorkPathSwitch[] = "/workpath:";

void updateWorkingDirectory(int argc, char *argv[])
{
  for (int i = 1; i < argc; i++)
  {
    char *arg = argv[i];
    if (strstr(arg, cWorkPathSwitch) == arg)
    {
      chdir(arg + sizeof(cWorkPathSwitch) - 1);
      return;
    }
  }
}

CApplication::CApplication(int argc, char *argv[])
{
  char szDir[MAXPATH];
  getcurdir(0, szDir);
  pSaveDir = (char*)malloc(strlen(szDir) + 1);
  strcpy(pSaveDir, szDir);
  updateWorkingDirectory(argc, argv);
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
