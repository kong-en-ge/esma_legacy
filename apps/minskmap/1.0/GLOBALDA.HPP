#ifndef GLOBDATA
#define GLOBDATA
#include "fstmod.hpp"
#include "globals.hpp"

extern PART_MODEL models[]; //части модели
extern int nmodels;         //число подмоделей модели
extern int N_on;
extern double XMINVC, YMINVC, XMAXVC, YMAXVC; //габаритные координаты  контр.окна вид
extern int XMINEC, YMINEC, XMAXEC, YMAXEC;
extern unsigned char SCALES;     //степень увеличения
extern GRwnd wndmain;
extern char *bram;
extern int GdEGA;

#endif