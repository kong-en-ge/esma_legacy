#ifndef GLOBDATA
#define GLOBDATA
#include "dbf.hpp"
#include "font.hpp"
#include "fstmod.hpp"
#include "globals.hpp"

#define WHITE 0
#define BLACK 15
#define MENUTOP 43
#define MENUBOT 188

class GRwnd;
class SET;
struct SCRIPT;
struct fonttype;
struct linesstyle;
struct FST_MODEL;
struct REC_RAZD
{
  char name[9], // DBF - file
      isactiv;
};
extern PART_MODEL models[]; //части модели
extern int nmodels;         //число подмоделей модели
extern REC_RAZD Cart[];
extern int NCart;
extern int All_Map;
extern int N_on;
extern int actdbf;
extern float CURRENTSCALE;
extern double XMING, YMING, XMAXG, YMAXG;     //Максимальные координаты
extern double XMINVC, YMINVC, XMAXVC, YMAXVC; //габаритные координаты  контр.окна вид
extern double XMINM, YMINM, XMAXM, YMAXM;     //габаритные координаты карты в мск
extern int XMINEC, YMINEC, XMAXEC, YMAXEC;
extern int xminec, yminec, xmaxec, ymaxec;
extern double _DXM, _DYM;
extern char SYSPATH[50];
extern char *LOC[];
extern unsigned char SCALES;     //степень увеличения
extern float SCALEMIN, SCALEMAX; //минимаксы масштабов
extern float SCALEO;             //основание степени
extern const int colordbf[];
// extern const unsigned int znakidbf[];
extern GRwnd wndmain;
class Menu_but_vrt;
struct ESIN_menu
{
  char *title;
  Menu_but_vrt *mn;
  int num_but;
  pascal ESIN_menu(char *titl, char *but_titl[], int key[]);
  pascal ~ESIN_menu();
  int pascal menu_key();
};
struct List_base
{
  char name[20];
  char file[9];
  char mark[2];
  char activ;
};
struct SET;
struct FLIB;
struct RAS;
extern SCRIPT scr;
extern fonttype ft;
extern FLIB flibrt[];
extern int flibsort[];
extern linesstyle fliblin[];
extern int LINsize;
extern RAS ras[];
extern unsigned FLIBsize;
extern char near *KLSKP[]; //смещение строк класс
extern char KLSK[];        //строки класс
extern far char ATCD[];
extern far unsigned ATCDP[];
struct atrname;
extern atrname ATRN[];
extern int ATRsize; //число возможных атрибутов
extern int RASsize;
extern FST_MODEL model;
extern List_base *LBS;
extern int LBSsize;
extern char *bram;
extern int GdEGA;

#endif