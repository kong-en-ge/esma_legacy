#include <string.h>
#include <dir.h>

#include "button.hpp"
#include "apputils.hpp"

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

int displaySplashBox(const char * version)
{
  GRwnd wnd2(170, 40, 470, 180, version, LIGHTBLUE, 0, 1);
  Menu_but mn;
  button b1(220, 155, 310, 171, 0, "�த������"),
         b2(330, 155, 420, 171, 1, "��室");
  mn + &b1 + &b2;
  wnd2.text("");
  wnd2.text("�ணࠬ�� ࠧࠡ�⠭� ��� '����'");
  wnd2.text("���㡫���   ��������   �. ����");
  wnd2.text("  ⥫.  (0172) - 20-96-36     ");
  wnd2.text("  䠪�. (0172) - 20-91-25 ESMA");
  wnd2.text("        (0172) - 27-00-13 ESMA");

  return mn.menu_key() == 0;
}
int confirmExit()
{
  GRwnd wnd(170, 100, 450, 180, "��������", LIGHTBLUE, 1, 1);
  button bt1(222, 155, 293, 171, 1, "��"),
         bt2(332, 155, 403, 171, -1, "He�");
  wnd.text("");
  wnd.text("�� ����⢨⥫쭮 ��� ���?");
  Menu_but menu;
  menu + &bt2 + &bt1;

  return menu.menu_key() != -1;
}
