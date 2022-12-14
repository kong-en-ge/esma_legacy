#include "graphics.h"
#include "globalda.hpp"
#include "fstmod.hpp"
#include "button.hpp"
#include "dbf.hpp"
#include "flibrt.hpp"
#include "set.hpp"
#include "atribut.hpp"

double XMING, YMING, XMAXG, YMAXG;     // габаритные координаты
double XMINVC, YMINVC, XMAXVC, YMAXVC; // габаритные координаты контр.окна вид
double XMINM, YMINM, XMAXM, YMAXM;     // габаритные координаты карты в мск
float CURRENTSCALE;
int XMINRE, YMINRE, XMAXRE, YMAXRE; // габаритные координаты окна раздела экр
int XMINEC, YMINEC, XMAXEC, YMAXEC; // габаритные координаты контр.окна карты на экр
int xminec, yminec, xmaxec, ymaxec;
double _DXM, _DYM;
SCRIPT scr;
fonttype ft;
unsigned char SCALES; // степень увеличения
const int colordbf[] = {RED, YELLOW, LIGHTBLUE, LIGHTGREEN, LIGHTCYAN};
double otkat[4 * 10];
int otkatn;
char *LOC[] = {"точ", "лин", "площ", "текст"};

float SCALEMIN = 50000, SCALEMAX = 10000000;
float SCALEO = 0.035; // ln основания степени

int All_Map;
int actdbf = 0;
FST_MODEL model;
List_base *LBS;
int LBSsize;
char *bram;
int GdEGA = 0;