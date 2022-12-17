#include "graphics.h"
#include "globalda.hpp"
#include "fstmod.hpp"
#include "button.hpp"
#include "dbf.hpp"
#include "flibrt.hpp"
#include "set.hpp"
#include "atribut.hpp"

double XMINVC, YMINVC, XMAXVC, YMAXVC; // габаритные координаты контр.окна вид
int XMINRE, YMINRE, XMAXRE, YMAXRE; // габаритные координаты окна раздела экр
int XMINEC, YMINEC, XMAXEC, YMAXEC; // габаритные координаты контр.окна карты на экр
int xminec, yminec, xmaxec, ymaxec;
fonttype ft;
unsigned char SCALES; // степень увеличения

char *bram;
int GdEGA = 0;