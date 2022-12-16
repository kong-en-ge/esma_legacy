#ifndef GLOBDATA
#define GLOBDATA
#include "dbf.hpp"
#include "font.hpp"
#include "fstmod.hpp"
#include "globals.hpp"

#define WHITE 0
#define BLACK 15
#define MENUTOP 43
#define MENUBOT 188

extern PART_MODEL models[]; //части модели
extern int nmodels;         //число подмоделей модели
extern int N_on;
extern double XMINVC, YMINVC, XMAXVC, YMAXVC; //габаритные координаты  контр.окна вид
extern double XMINM, YMINM, XMAXM, YMAXM;     //габаритные координаты карты в мск
extern int XMINEC, YMINEC, XMAXEC, YMAXEC;
extern int xminec, yminec, xmaxec, ymaxec;
extern char *LOC[];
extern unsigned char SCALES;     //степень увеличения
extern GRwnd wndmain;
extern fonttype ft;
extern FLIB flibrt[];
extern int flibsort[];
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