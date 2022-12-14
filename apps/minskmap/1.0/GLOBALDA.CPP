#include "graphics.h"
#include "globalda.hpp"

double XMING, YMING, XMAXG, YMAXG;     //габаритные координаты
double XMINVC, YMINVC, XMAXVC, YMAXVC; //габаритные координаты контр.окна вид
double XMINM, YMINM, XMAXM, YMAXM;     //габаритные координаты карты в мск
float CURRENTSCALE;
int XMINRE, YMINRE, XMAXRE, YMAXRE; //габаритные координаты окна раздела экр
int XMINEC, YMINEC, XMAXEC, YMAXEC; //габаритные координаты контр.окна карты на экр
int xminec, yminec, xmaxec, ymaxec;
double _DXM, _DYM;
SCRIPT scr;
fonttype ft;
unsigned char SCALES; //степень увеличения
const int colordbf[] = {RED, YELLOW, LIGHTBLUE, LIGHTGREEN, LIGHTCYAN};
char *LOC[] = {"точ", "лин", "площ", "текст"};

float SCALEMIN = 400, SCALEMAX = 200000;
float SCALEO = 0.035; // ln основания степени

int All_Map;
int N_on = 0;
int actdbf = 0;
FST_MODEL model;
List_base *LBS;
int LBSsize;
char *bram;
int GdEGA = 0;