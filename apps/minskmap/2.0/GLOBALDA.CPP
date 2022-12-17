#include "graphics.h"
#include "globalda.hpp"
#include "fstmod.hpp"
#include "button.hpp"
#include "dbf.hpp"
//#include "test.hpp"

double XMINVC, YMINVC, XMAXVC, YMAXVC; //габаритные координаты контр.окна вид
int XMINRE, YMINRE, XMAXRE, YMAXRE; //габаритные координаты окна раздела экр
int XMINEC, YMINEC, XMAXEC, YMAXEC; //габаритные координаты контр.окна карты на экр
unsigned char SCALES; //степень увеличения

int N_on = 0;
char *bram;
int GdEGA = 0;
int pr = 0, xo, yo, retot, kolmet, koluz, comuz, xco, yco, xck, yck, prmt, uzl;
char stt[40], nmot1[90], ost[40];
OBSTOP *obstop, *otkuda;
METRO *metro;
UZEL *uzel;
UZLY *uzly;