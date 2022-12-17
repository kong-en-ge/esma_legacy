#include "globals.hpp"

double XMING, YMING, XMAXG, YMAXG;     // габаритные координаты
double XMINV, YMINV, XMAXV, YMAXV;     // габаритные координаты окна вид
double XMINM, YMINM, XMAXM, YMAXM;     //габаритные координаты карты в мск
int XMINE, YMINE, XMAXE, YMAXE;     // габаритные координаты окна карты на экр
double XMINVC, YMINVC, XMAXVC, YMAXVC; // габаритные координаты контр.окна вид
int XMINRE, YMINRE, XMAXRE, YMAXRE; // габаритные координаты окна раздела экр
int XMINEC, YMINEC, XMAXEC, YMAXEC; // габаритные координаты контр.окна карты на экр
unsigned char SCALES; // степень увеличения
int XMAXMAPE = 479;
int YMAXMAPE = 349;
double _DXM, _DYM;
int DXM;

SCRIPT scr;
fonttype ft;

float CURRENTSCALE;
float SCALEMIN, SCALEMAX;
float SCALEO;

int All_Map;

double otkat[4 * MAX_STORED_VIEWS];
int otkatn;

FST_MODEL model;

int _xMK = 240, _yMK = 240;

const int colordbf[] = {RED, YELLOW, LIGHTBLUE, LIGHTGREEN, LIGHTCYAN};
List_base *LBS;
int LBSsize;
int actdbf = 0;

char const * LBSFilename;

CIndexMapImage indexMapImage;
int GdEGA = 0;