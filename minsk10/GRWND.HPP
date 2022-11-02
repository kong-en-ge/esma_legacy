#ifndef GRWND
  #define GRWND
#include <mem.h>
#include "graphics.h"
int const BORD_S = 6;
#define BLACK 15
#define WHITE 0

struct SVS {
 struct viewporttype     view;
 struct fillsettingstype fill;
 int    color;
};
void pascal savesettings(SVS *svs);
void pascal restoresettings(SVS *svs);

struct REGION {
 int left,top,right,bot;
};

int pascal xyinregion(int x,int y,REGION *reg);
int pascal regionregion(REGION *reg1,REGION *reg2);
int pascal xydregion(int x,int y,int delta,REGION *reg);

struct Sreg : public REGION {
  int  rem;
  char *mem;
  pascal Sreg(int left,int top,int right,int bot,int rem);
  void pascal store();
  void pascal restore();
  void pascal restore(int color);
  pascal ~Sreg();
};

struct GRwnd : public Sreg {
  int nst,titlec;    //номер строки,цвет фона заголовка
  char *title  ;     //заголовок
  pascal GRwnd(int xl,int yt,int xr,int yb,char *,
	       int titlec_=LIGHTBLUE,int rm=0,int drw=0);
  void pascal draw(void);
  void pascal text(char *string,int center=0);
  void pascal textxy(int x,int y,char *string,int color=BLACK);
  int  pascal readst(char *,int Maxs,char *firststr=NULL,int Maxs2=0,
		     int atrib=BLACK|(LIGHTGRAY<<4),int x_posit=0);
  int  pascal readfloat(float *fl,float minfl,float maxfl,int MAXL,
			char *firststr=NULL,int x_posit=0);
  int  pascal readdouble(double *fl,double minfl,double maxfl,int MAXL,
			char *firststr=NULL,int x_posit=0);
  void pascal clear(void);
  void pascal clear_str(int n); //очистить строку n
  void pascal clear_str(int n,int start,int sym);//sym символов на n строке c start


};

#endif