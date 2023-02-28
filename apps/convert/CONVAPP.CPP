#include <alloc.h>
#include <string.h>
#include <dir.h>
#include <stdio.h>
#include <iostream.h>

#include "convapp.hpp"
#include "mouse.hpp"
#include "button.hpp"
#include "bigpath.hpp"
#include "apputils.hpp"

int coder(char const *, char const *, CErrorHandler*);
void convertClassifier(char const *);
int loadClassifier(char const *, CErrorHandler*);

const char *errorMessages[] =
{
  "Ошибка открытия исходной геобазы",     // 0
  "Ошибка чтения исходной геобазы",       // 1
  "Ошибка записи выходной геобазы",       // 2
  "Ошибка закрытия выходной геобазы",     // 3
  "Нет памяти для работы программы",      // 4
  "Перегонка модели завершилась успешно", // 5
  "Объект вне классификатора",
  "Атрибут вне классификатора",          // 7
  "Кодовый атрибут вне классификатора",  // 8
  "Ошибка чтения файлов классификатора", // 9
  "Числовой атрибут не число"
};

void errorHandler(void* /*context*/, int code, char const* message)
{
  displayError(code, message);
}

void errorHandlerC(void* /*context*/, int code, char const* message)
{
  cout << "ERR" << code << ":" << message << endl;
}

int Init(CErrorHandler* onError)
{
  setfillstyle(1, BLACK);
  bar(0, 0, getmaxx(), getmaxy());
  setcolor(BLACK);
  init_mouse();
  asm mov ax, 0xf
  asm mov cx, 10
  asm mov dx, 8
  asm int 0x33
  mouse_show(ON);
  if (farcoreleft() <= 0x20000l)
  {
    handleError(onError, 7);
    return -1;
  }

  char path[MAXPATH];
  path[0] = '\\';
  getcurdir(0, path + 1);
  strcat(path, "\\");

  return loadClassifier(path, onError);
}

int findCommandLineParam(int argc, char *argv[], char const* param, char* value = NULL)
{
  char const* arg;
  int paramLen = strlen(param);

  for (int i = 1; i < argc; i++)
  {
    arg = argv[i];

    if (*arg != '/' && *arg != '-')
      continue;

    if (strncmpi(arg + 1, param, paramLen))
      continue;

    if (arg[paramLen + 1] != 0)
    {
      if (arg[paramLen + 1] != ':')
        continue;

      if (value != NULL)
        strcpy(value, arg + paramLen + 2);
    }

    return 1;
  }

  return 0;
}

/*************************
 * CConverterApplication *
 *************************/

CConverterApplication::CConverterApplication(int argc, char *argv[]) :
  CGraphicApplication(argc, argv)
{
  onError = new CErrorHandler(errorHandler);
}

CConverterApplication::~CConverterApplication(void)
{
  delete onError;
}

void CConverterApplication::convertMapData(void)
{
  Big_Menu_model mn(200, 100, 440, 10, "Выберите модель", 0);
  mn.set_colon(2);
  while (mn.menu_key() != -1)
  {
    mn.restore(BLACK);
    char *t = strchr(mn.path, '.');
    *t = 0;
    coder(mn.path, t + 1, getErrorHandler());
  }
}

char const * CConverterApplication::getVersion(void) const
{
  return "Fast Converter";
}

int CConverterApplication::runUI(void)
{
  setErrorMessages(errorMessages, sizeof(errorMessages) / sizeof(errorMessages[0]));

  int ret = Init(getErrorHandler());
  if (ret)
    return -1;

  char const *ch[] = {"Кодирование", "ASM-file классификатора", "Выход"};
  int key[] = {0, 1, -1};
  Menu_but_vrt mn(220, 100, 420, ch, key, 3, 0);
  do
  {
    mn.restore(BLACK);
    ret = mn.menu_key();
    switch (ret)
    {
    case 0:
      convertMapData();
      break;

    case 1:
      convertClassifier("flib_txt.asm");
      break;

    }
  }
  while (ret != -1);

  return 0;
}

/********************************
 * CConverterConsoleApplication *
 ********************************/

CConverterConsoleApplication::CConverterConsoleApplication(int argc, char *argv[]) :
  CApplication(argc, argv)
{
  _argc = argc;
  _argv = argv;
}

void CConverterConsoleApplication::convertClassifier(void)
{
  char filename[MAXPATH];
  if (!findCommandLineParam("oc", filename))
    return;

  if (strlen(filename) == 0)
    strcpy(filename, "flib_txt.asm");

  ::convertClassifier(filename);
}

int CConverterConsoleApplication::findCommandLineParam(
  char const* param, char* value) const
{
  return ::findCommandLineParam(_argc, _argv, param, value);
}

int CConverterConsoleApplication::loadClassifier(void)
{
  char path[MAXPATH];
  if (!findCommandLineParam("classifier", path))
  {
    path[0] = '\\';
    getcurdir(0, path + 1);
    strcat(path, "\\");
  }

  CErrorHandler onError(errorHandlerC);

  return ::loadClassifier(path, &onError);
}

int CConverterConsoleApplication::run(void)
{
  if (!loadClassifier())
  {
    cout << "Classifier has been loaded" << endl;
    convertClassifier();
  }
  return 0;
}

int main(int argc, char *argv[])
{
  CApplication *app;
  if (!findCommandLineParam(argc, argv, "console"))
    app = new CConverterApplication(argc, argv);
  else
    app = new CConverterConsoleApplication(argc, argv);

  int retValue = app->run();
  delete app;
  return retValue;
}
