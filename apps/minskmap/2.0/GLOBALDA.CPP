#include "graphics.h"
#include "globalda.hpp"
#include "fstmod.hpp"
#include "button.hpp"
#include "dbf.hpp"
#include "flibrt.hpp"
//#include "test.hpp"
#include "set.hpp"
#include "atribut.hpp"

double XMINVC, YMINVC, XMAXVC, YMAXVC; //габаритные координаты контр.окна вид
int XMINRE, YMINRE, XMAXRE, YMAXRE; //габаритные координаты окна раздела экр
int XMINEC, YMINEC, XMAXEC, YMAXEC; //габаритные координаты контр.окна карты на экр
int xminec, yminec, xmaxec, ymaxec;
fonttype ft;
unsigned char SCALES; //степень увеличения
char *LOC[] = {"точ", "лин", "площ", "текст"};

int N_on = 0;
int actdbf = 0;
char *bram;
int GdEGA = 0;
int pr = 0, xo, yo, retot, kolmet, koluz, comuz, xco, yco, xck, yck, prmt, uzl;
char stt[40], nmot1[90], ost[40];
OBSTOP *obstop, *otkuda;
METRO *metro;
UZEL *uzel;
UZLY *uzly;