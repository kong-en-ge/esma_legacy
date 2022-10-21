#ifndef atribhpp
 #define atribhpp
//максимальный размер атрибута
#define MAX_ATR_SIZE  70

#define ERROR_STR  -1

#define	O_NOT	0	//Отсутствие атрибута
#define O_IS	1       //Наличие атрибута
#define O_EQ	2	//==
#define O_NE	3       //!=
#define O_GR	4       //>
#define O_GE	5       //>=
#define O_LS	6       //<
#define O_LE	7	//<=

//#define P_OR	0
//#define P_AND	1

#include "GEOMODEL.hpp"
//Структура атрибут-значение
struct atribut {
  char atr[3];
  char zna[MAX_ATR_SIZE+1];
};
//Структура записи файла имен атрибутов
//структура для получения атрибута из файла характеристики модели данных
struct mk_atr : public atribut {
  char *har;//указатель в файле атрибутов
	    //указывает на текущий атрибут
};

//функция получения атрибута из файла характеристики модели данных
//получает текущий атрибут в структуру m_a
//возвращает 0 - o'k, -1 - error ,1 - этот атрибут последний
int make_atr(mk_atr *m_a);

//вспомогательная структура для поиска по атрибутам
struct f_attr : public atribut {
  char type;   //тип атрибута
  int  prp;    //принцип поиска (наличие - отсутствие - совпадение - несовпадение);
//  int  orp;    //выражение (и - или)
  int  tpr;    //текущий признак
};
/*
//структура для поиска по одному атрибуту
struct FINDatr : public f_attr {
  char clcod[10];
  int  startobj;
  int findobj(GEO_MODEL *model);
};

//структура для поиска по атрибутам
struct FINDobj {
  char clcod[10][9];
  int  ncod;
  f_attr atr[5];
  int  natr;
  int  startobj;
  FINDobj();
  void add_clcod(char *clc);
  void add_f_attr(char *atr,char *zna,int prp,int orp);
  int findobj(GEO_MODEL *model);
};
*/

//структура для поиска по нескольким атрибутам без кода
struct FINDatrs {
  f_attr atr[5];
  int  natr;
  unsigned  blok,smes; //Начальный блок и смещение в блоке по файлу атрибутов
};

int findobj(GEO_MODEL *model,FINDatrs *fa);

#endif