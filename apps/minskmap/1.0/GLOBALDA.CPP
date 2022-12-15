#include "graphics.h"
#include "globalda.hpp"

double XMINVC, YMINVC, XMAXVC, YMAXVC; //габаритные координаты контр.окна вид
int XMINRE, YMINRE, XMAXRE, YMAXRE; //габаритные координаты окна раздела экр
int XMINEC, YMINEC, XMAXEC, YMAXEC; //габаритные координаты контр.окна карты на экр
int xminec, yminec, xmaxec, ymaxec;
SCRIPT scr;
fonttype ft;
unsigned char SCALES; //степень увеличения
const int colordbf[] = {RED, YELLOW, LIGHTBLUE, LIGHTGREEN, LIGHTCYAN};
char *LOC[] = {"точ", "лин", "площ", "текст"};

int N_on = 0;
int actdbf = 0;
List_base *LBS;
int LBSsize;
char *bram;
int GdEGA = 0;