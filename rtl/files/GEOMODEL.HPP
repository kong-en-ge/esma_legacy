#ifndef geomodel
#define geomodel
#include <memory.h>

#include "files.hpp"

struct SPR
{
  unsigned rez1, nom;
  char clcod[8];
  unsigned char loc, rez2;
  int xmin, ymin, xmax, ymax;
  unsigned stblm, enblm, stblh;
  int rez[2];
};

struct FORM
{
  char nom[16],        //Код номенклатуры
      kob[2],          //Код обработки
      tikm[2],         //Тип ИКМ
      nosi[2],         //Номер оси
      time[10],        //Время создания
      nkom[2],         //Номер комплекса
      nrm[2],          //Номер рабочего места
      nseg[2],         //Номер сегмента
      rez1[4];         //Резерв
  unsigned maxn;       //Максимальный номер объекта
  unsigned rez2[10];   //Резерв
  char nmen[2];        //Номер меню
  unsigned scale;      //Знаменатель масштаба /1000
  int pak,             //Признак автоматического контроля
      rez3;            //Резерв
  char sd[2],          //Шаг дискретизации
      sp[4];           //Шаг планшета(не используется)
  int rez4[9];         //Резерв
  float sgp;           //Широта главной параллели
  int xuz, yuz,        //Координаты углов карты
      xsz, ysz,        //в дискретах
      xsv, ysv,        //
      xuv, yuv;        //
  float xuzf, yuzf,    //Координаты углов карты
      xszf, yszf,      //в географических координатах
      xsvf, ysvf,      //
      xuvf, yuvf;      //
  float xuzt, yuzt,    //Теоретические координаты углов рамки
      xszt, yszt,      //
      xsvt, ysvt,      //
      xuvt, yuvt;      //
  char cp[2];          //Код проекции
  float par1,          //Первый параметр проекции
      par2,            //Второй
      par3;            //Третий
  long zscal;          //Знаменатель масштаба
  char gsys[3],        //Геодезическая система
      al[3];           //Эллипсоид
  float shnt, dlnt;    //Широта+Долгота начальной точки
  float xuzp, yuzp,    //Прямоугольные координаты углов
      xszp, yszp,      // x изменяется вверх
      xsvp, ysvp,      //
      xuvp, yuvp;      //
  char pkoor[3],       //Представление координат
      ezmk[3];         //Единицы измерения координат
  float rkoor;         //Разрешение координат
  char temobl;         //Тематическая область
  char ezmhh[3];       //Единицы измерения высотной основы
  float razhh;         //Высота сечения рельефа
  int rez5[89];        //Резерв
  unsigned int u0, u1, //Координаты дигитайзера
      u2, u3,
      u4, u5,
      u6, u7;
  int rez6[22]; //Резерв
  int razd,     //Разделитель, признак нового формуляра
      rez7[3];  //Резерв
  char sertc,   //Сертификатор С структуры
      sertu,    //Сертификатор узлов
      serts,    //Сертификатор семантики
      sertm;    //Сертификатор метрики
};

unsigned size_methar(const void *buf);
void gab_ram(const void *metr, SPR *spr);
int writemet_har(const void *met_har, Files *f, unsigned *fbl, unsigned *ebl);
int readmet_har(void **met_har, Files *f, int no, int fbl, int ebl);
extern "C" double pascal square_metr(void const *);
extern "C" int pascal orient_metr(void const *);

//возврат -1 - нет справки,-2 - нет метрики,-3 - нет характерист
int isexistmodel(const char *path, char *mod);

struct FINDHAR
{
  char *atr;      //Атрибут поиска (без значения или с точным значнием)
  int blok, smes; //Начальный блок и смещение в блоке
  FINDHAR();
};

inline FINDHAR::FINDHAR()
{
  memset(this, 0, sizeof(FINDHAR));
}

class GEO_MODEL
{
private:
  Files hsp, hmt, hhr; // handle-описатели файлов

public:
  int xminm, yminm, xmaxm, ymaxm;
  int xminw, yminw, xmaxw, ymaxw;
  char path[80], mod[3];
  int maxn;

  GEO_MODEL();
  ~GEO_MODEL();
  int openexist(const char *name, const char *model, int spbsize = 320,
                int mtsize = 80, int hrsize = 20, int readonly = 0);
  int close();
  int readform(void *buff);
  int writeform(const void *buff);
  int readsprb(void **sprp, unsigned no); //Читает из буфера файла
  int readspr(void *spr, unsigned no);    //Читает в буфер пользоват
  int readmet(void **metp, unsigned no);  //Читает из буфера файла
  int readhar(void **harp, unsigned no);  //Читает из буфера файла
  int readmets(void **metp, SPR *spr);    //Читает из буфера файла
  int readhars(void **harp, SPR *spr);    //Читает из буфера файла
  int readhars(void **harp, SPR *spr, char *attr);
  int readhars(SPR *spr, char *attr, char *str, int n);
  //помещает атрибут в строку str не более n символов
  int readdoublehars(SPR *spr, char *attr, double *at);
  int findhar(FINDHAR *f);
  //Возвращает -1 - ошибка ; 0 - не найден объект ; иначе номер объекта

  //записывается метрика из внутреннего буфера файла
  int writemetbuff(SPR *spr);
  int writeharbuff(SPR *spr);
  int writespr(const SPR *spr);
  int writemets(const void *metr, SPR *spr);
  int writehars(const void *har, SPR *spr);
  //с построением габаритной рамки
  int writemetsgb(const void *metr, SPR *spr);
};

#endif