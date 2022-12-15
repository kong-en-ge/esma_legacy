#ifndef __GLOBALS_HPP__
#define __GLOBALS_HPP__

#include "fstmod.hpp"

#define MAX_STORED_VIEWS 10

extern int XMAXMAPE, YMAXMAPE;
extern double XMING, YMING, XMAXG, YMAXG;     // Максимальные координаты
extern double XMINM, YMINM, XMAXM, YMAXM;     //габаритные координаты карты в мск
extern double XMINV, YMINV, XMAXV, YMAXV;     // габаритные координаты окна вид
extern int XMINE, YMINE, XMAXE, YMAXE;

extern double _DXM, _DYM;

extern float CURRENTSCALE;
extern float SCALEMIN, SCALEMAX; // min/max scale
extern float SCALEO; // основание степени

extern int All_Map;

extern double otkat[];
extern int otkatn;

extern FST_MODEL model;

extern int _xMK, _yMK;

#endif
