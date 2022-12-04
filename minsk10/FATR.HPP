#ifndef FATRHPP
#define FATRHPP
#include "atribut.hpp"
#include "fastmod.hpp"

struct atrib_f
{
  int natr; //Номер атрибута
  union
  {
    char zns[MAX_ATR_SIZE + 1];
    float znn;
    unsigned znk;
  };
};

struct mk_atrf : public atrib_f
{
  char *har; //указатель в файле атрибутов
             //указывает на текущий атрибут
};

//функция получения атрибута из файла характеристики модели данных
//получает текущий атрибут в структуру m_a
//возвращает 0 - o'k, -1 - error ,1 - этот атрибут последний
int make_atr(mk_atrf *m_a);

//структура для поиска по нескольким атрибутам без кода
struct FINDatrf
{
  struct : public atrib_f
  {
    int tpr;
  } atr[5];
  int natr;
  unsigned blok, smes; //Начальный блок и смещение в блоке по файлу атрибутов
};

//Для первого поиска присвоить fa.blok && fa.smes значениями
// mod.fm.sbh и mod.fm.ssh
//Поиск только по равенству (значения fa.atr.prp не проверяется)
//возвращает 0xffff в случае ошибки или неудачного поиска или
//номер последнего найденного объекта. Т.к. записи объектов
//упорядочены по возрастания номеров то поиск ведется также по
//порядку
unsigned findobj(FAST_MODEL &mod, FINDatrf &fa);
#endif