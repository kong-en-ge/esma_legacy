#ifndef __GLOBALS_HPP__
#define __GLOBALS_HPP__

#include "graphics.h"
#include "fstmod.hpp"
#include "font.hpp"
#include "flibrt.hpp"

#define MAX_STORED_VIEWS 10

struct List_base
{
  char name[20];
  char file[9];
  char mark[2];
  char activ;
};

extern int XMAXMAPE, YMAXMAPE;
extern double XMING, YMING, XMAXG, YMAXG;     // Максимальные координаты
extern double XMINM, YMINM, XMAXM, YMAXM;     //габаритные координаты карты в мск
extern double XMINV, YMINV, XMAXV, YMAXV;     // габаритные координаты окна вид
extern int XMINE, YMINE, XMAXE, YMAXE;

extern double _DXM, _DYM;
extern int DXM;

extern SCRIPT scr;

extern float CURRENTSCALE;
extern float SCALEMIN, SCALEMAX; // min/max scale
extern float SCALEO; // основание степени

extern int All_Map;

extern double otkat[];
extern int otkatn;

extern FST_MODEL model;

extern int _xMK, _yMK;

extern const int colordbf[];
extern List_base *LBS;
extern int LBSsize;
extern char const * LBSFilename;
extern int actdbf;

// external classifier data
extern linesstyle fliblin[];
extern RAS ras[];
extern int RASsize;

#endif
