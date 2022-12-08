#include <string.h>
#include <dir.h>

#include "..\..\library\apputils.hpp"

extern int org_main(int argc, char *argv[]);

const char cWorkPathSwitch[] = "/workpath:";

void UpdateWorkingDirectory(int argc, char *argv[])
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

int runApplication(int argc, char *argv[], APP_FUNC appFunc)
{
  char szSaveDir[MAXDIR];

  getcurdir(0, szSaveDir);
  UpdateWorkingDirectory(argc, argv);

  int result = appFunc(argc, argv);

  chdir("\\");
  chdir(szSaveDir);

  return result;

}

int main(int argc, char *argv[])
{
  return runApplication(argc, argv, org_main);
}
