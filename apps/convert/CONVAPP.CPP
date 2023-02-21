#include <string.h>

#include "convapp.hpp"
#include "button.hpp"
#include "bigpath.hpp"
#include "apputils.hpp"

int Init();

int coder(char const *, char const *);
void convertClassifier(void);

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

CConverterApplication::CConverterApplication(int argc, char *argv[]) :
  CGraphicApplication(argc, argv)
{
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
    coder(mn.path, t + 1);
  }
}

char const * CConverterApplication::getVersion(void) const
{
  return "Fast Converter";
}

int CConverterApplication::runUI(void)
{
  setErrorMessages(errorMessages, sizeof(errorMessages) / sizeof(errorMessages[0]));

  int ret = Init();
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
      convertClassifier();
      break;

    }
  }
  while (ret != -1);

  return 0;
}

int main(int argc, char *argv[])
{
  CApplication *app = new CConverterApplication(argc, argv);
  int retValue = app->run();
  delete app;
  return retValue;
}
