#ifndef FASTMODEL
#define FASTMODEL

#include "buffiles.hpp"
/*
 Файл справки - первая буква расширения - 'c'.
 записи - F_FORM,далее F_SPR для объектов с нулевого номера.
 Объекты упорядочены по порядку вывода.
 Формуляр считывается при открытии модели.
 Файл метрики - первая буква расширения - 'd'.
 записи - 0xC018,номер объекта,(с 0),X1,Y1,...,XN,YN.
 Объекты с характером локализации 4 (подпись) сразу после записи метрики
 имеют запись атрибута 09 в формате 0xC018,номер объекта,подпись.
 Подпись оканчивается 0, длина выравнена на границу двойного слова.
 Длина подписи включена в длину метрики объекта.
 Записи атрибутов - начинаются за последней записью метрики
 (блок sbh-смещение ssh по F_FORM)
 Структура -0xC018,номер объекта,атрибут1,...,атрибутN.
 Атрибут - запись вида Nатр (1байт,с 1-го по классификатору),
 длина атрибута и признак последнего атрибута объекта(7 мл. бит
 и старший бит соответсвенно), значение атрибута (текстовый - текст,
 ограниченный 0, числовой - число типа float, кодовый - номер кода
 атрибута по классификатору. Записи атрибутов объекта выравнены на
 границу слова. За последней записью - двойной разделитель.
*/

struct F_FORM
{
  unsigned maxn;                     //Максимальный номер
  int xmin, ymin, xmax, ymax;        //Габариты в дискретах
  double xminm, yminm, xmaxm, ymaxm; //Габариты в метрах
  unsigned sbh;                      //Начальные блок и смещение
  unsigned ssh;                      //характеристик в файле метрики
};

struct F_SPR
{
  int xmin, ymin, xmax, ymax; // габаритная рамка
  unsigned sbm;               // нач блок метр
  unsigned lnm;               // длина метрики
  unsigned sbh;               // нач блок хар
  unsigned lnh;               // длина характеристики
  unsigned char ssm;          // нач смещ метр
  unsigned char ssh;          // нач смещ хар
  unsigned numincl;           // номер в классификаторе
};

struct FAST_MODEL
{
  F_FORM fm;
  int xminw, yminw, xmaxw, ymaxw;
  XMSFiles hs, hm, hh; // handles справки и метрики

  ~FAST_MODEL();

  int open(const char *name, const char *model);
  int close();
  int readspr(F_SPR *spr, unsigned no);
  int readspr(F_SPR **spr, unsigned no);
  int readmets(int **metp, F_SPR *spr);
  int readhars(int **harp, F_SPR *spr);
};

inline int FAST_MODEL::readspr(F_SPR *spr, unsigned no)
{
  return hs.readtobuf(no, no, (void *)spr);
}

inline int FAST_MODEL::readspr(F_SPR **spr, unsigned no)
{
  return hs.readtobuf(no, no, (void **)spr);
}

#endif