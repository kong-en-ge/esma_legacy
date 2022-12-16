#ifndef GLOBDATA
#define GLOBDATA
#include "dbf.hpp"
#include "font.hpp"
#include "fstmod.hpp"
#include "globals.hpp"
#include "set.hpp"

#define WHITE 0
#define BLACK 15
#define MENUTOP 43
#define MENUBOT 188

extern PART_MODEL models[]; //части модели
extern int nmodels;         //число подмоделей модели
extern int N_on;
extern double XMING, YMING, XMAXG, YMAXG;     //Максимальные координаты
extern double XMINVC, YMINVC, XMAXVC, YMAXVC; //габаритные координаты  контр.окна вид
extern int XMINEC, YMINEC, XMAXEC, YMAXEC;
extern int xminec, yminec, xmaxec, ymaxec;
extern char *LOC[];
extern unsigned char SCALES;     //степень увеличения
extern GRwnd wndmain;
struct OBSTOP
{
  int nobj;
  float d;
  char vt;
};
struct PERES
{
  char stperes[40];
  char ot_kd[40];
  char nm[55];
  char vt;
  float d;
};
//	      int nperes;
//	      int not_kd;     };
struct PERESU
{
  char stperes[40];
  char kd[40];
  char ot[40];
  char nmot[90];
  char nmkd[90];
  float d;
};
/*	       char vtot,vtkd;
         float d;
         int nperes,pcol;
         int xot,yot;
         int xkd,ykd;     };   */
struct TWOPERES
{
  char stperes1[40], stperes2[40];
  char kd[40];
  char ot[40];
  char nmot[90];
  char nmkd[90];
  char nmuz[90];
  float d;
};
extern OBSTOP *obstop, *otkuda;
struct METRO
{
  char smetro[40];
  int x, y, n;
};
extern METRO *metro;
struct UZEL
{
  char suzel[40], nmu0[25], nmu1[55], nmu2[55], nmu3[55];
  int x, y, n;
  float d;
};
extern UZEL *uzel;
struct UZLY
{
  int x, y, n;
};
extern UZLY *uzly;
extern int pr, xo, yo, retot, kolmet, koluz, comuz, xco, yco, xck, yck, prmt, uzl;
extern char stt[40], nmot1[90], ost[40];
extern fonttype ft;
extern FLIB flibrt[];
extern int flibsort[];
extern SET fliben;
extern int LINsize;
extern unsigned FLIBsize;
extern char near *KLSKP[]; //смещение строк класс
extern char KLSK[];        //строки класс
extern far char ATCD[];
extern far unsigned ATCDP[];
struct atrname;
extern atrname ATRN[];
extern int ATRsize; //число возможных атрибутов
extern char *bram;
extern int GdEGA;

#endif