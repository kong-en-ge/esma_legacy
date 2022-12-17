#include "graphics.h"
#include "globalda.hpp"

double XMINVC, YMINVC, XMAXVC, YMAXVC; //габаритные координаты контр.окна вид
int XMINRE, YMINRE, XMAXRE, YMAXRE; //габаритные координаты окна раздела экр
int XMINEC, YMINEC, XMAXEC, YMAXEC; //габаритные координаты контр.окна карты на экр
int xminec, yminec, xmaxec, ymaxec;
unsigned char SCALES; //степень увеличения

int N_on = 0;
char *bram;
int GdEGA = 0;