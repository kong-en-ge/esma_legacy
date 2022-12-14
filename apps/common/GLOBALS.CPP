#include "globals.hpp"

double XMING, YMING, XMAXG, YMAXG;     // габаритные координаты
double XMINV, YMINV, XMAXV, YMAXV;     // габаритные координаты окна вид
int XMINE, YMINE, XMAXE, YMAXE;     // габаритные координаты окна карты на экр
int XMAXMAPE = 479;
int YMAXMAPE = 349;

float CURRENTSCALE;
float SCALEMIN, SCALEMAX;
float SCALEO;

int All_Map;

double otkat[4 * MAX_STORED_VIEWS];
int otkatn;
