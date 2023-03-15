#include <alloc.h>
#include <string.h>
#include <dir.h>
#include <stdio.h>
#include <iostream.h>

#include "utils.hpp"
#include "convapp.hpp"
#include "mouse.hpp"
#include "button.hpp"
#include "bigpath.hpp"
#include "apputils.hpp"
#include "convert.hpp"

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
  cout << "ERR" << code << ":" << errorMessages[code];
  if (message)
    cout << " " << message << endl;
  cout << endl;
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

    MapConverterData *mapConverterData = new MapConverterData;
    strcpy(mapConverterData->mapSrc, mn.path);
    strncpy(mapConverterData->mapSrcExt, t + 1, 2);
    strcpy(mapConverterData->mapDst, mapConverterData->mapSrc);
    strcpy(mapConverterData->mapDstExt, mapConverterData->mapSrcExt);
    strcpy(mapConverterData->objectDirectory, mapConverterData->mapSrc);
    strcat(mapConverterData->objectDirectory, ".asm");
    encodeMapData(mapConverterData, getErrorHandler());

    delete mapConverterData;
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
  onError = new CErrorHandler(errorHandlerC);
}

CConverterConsoleApplication::~CConverterConsoleApplication(void)
{
  delete onError;
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

void CConverterConsoleApplication::convertMapData(void)
{
  char tempStr[MAXPATH];
  MapConverterData* mapConverterData = new MapConverterData;
  memset(mapConverterData, 0, sizeof(MapConverterData));

  if (findCommandLineParam("ms", mapConverterData->mapSrc) && strlen(mapConverterData->mapSrc) != 0)
  {
    if (findCommandLineParam("es", tempStr))
      strncpy(mapConverterData->mapSrcExt, tempStr, 2);
    else
      strcpy(mapConverterData->mapSrcExt, "00");

    if (!findCommandLineParam("md", mapConverterData->mapDst))
      strcpy(mapConverterData->mapDst, mapConverterData->mapSrc);

    if (findCommandLineParam("ed", tempStr))
      strncpy(mapConverterData->mapDstExt, tempStr, 2);
    else
      strcpy(mapConverterData->mapDstExt, mapConverterData->mapSrcExt);

    if (!findCommandLineParam("od", mapConverterData->objectDirectory))
    {
      strcpy(mapConverterData->objectDirectory, mapConverterData->mapDst);
      strcat(mapConverterData->objectDirectory, ".asm");
    }

    encodeMapData(mapConverterData, getErrorHandler());
  }

  delete mapConverterData;
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

  return ::loadClassifier(path, getErrorHandler());
}

int CConverterConsoleApplication::run(void)
{
  if (!loadClassifier())
  {
    cout << "Classifier has been loaded" << endl;
    convertClassifier();
    convertMapData();
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
