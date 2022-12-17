#ifndef GLOBDATA
#define GLOBDATA
#include "font.hpp"
#include "fstmod.hpp"
#include "globals.hpp"

extern PART_MODEL models[]; //части модели
extern int nmodels;         //число подмоделей модели
extern int N_on;
extern double XMINVC, YMINVC, XMAXVC, YMAXVC; //габаритные координаты  контр.окна вид
extern double XMINM, YMINM, XMAXM, YMAXM;     //габаритные координаты карты в мск
extern int XMINEC, YMINEC, XMAXEC, YMAXEC;
extern int xminec, yminec, xmaxec, ymaxec;
extern unsigned char SCALES;     //степень увеличения
extern GRwnd wndmain;
extern fonttype ft;
extern char *bram;
extern int GdEGA;

#endif