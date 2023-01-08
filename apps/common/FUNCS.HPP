#ifndef __FUNCS_HPP__
#define __FUNCS_HPP__

#include <stdlib.h>

#include "txtpacks.hpp"
#include "fastmod.hpp"

void store_otkat(void);
void restore_otkat(void);
int upcase(int symb);
void rasst(void);
void help(const char* help_file);

int next_model(int index, int ignoreScaleFilter = 0);

double YEKRAZD(int x);
double XEKRAZD(int y);
int XRAZEKR(float y);
int YRAZEKR(float x);
double YDISRAZ(int x);
double XDISRAZ(int y);
int XRAZDIS(double y);
int YRAZDIS(double x);
int DXEKRDIS(int dx);
double DXDISEKR(int dx);

struct OBJND
{
  int nobj;
  float d;
  int color;
};

int selectObjectPoint(double *x, double *y);
int chooseObject(const OBJND *obj, int nobj);
void whatisit(void);
void displayObjectInfo(int objIndex);
int ATR(TEXT_PACK_MENUS *tp, int obj);

int checkAttributeName(int index, char const* name);
int checkAttributeDef(int index, char const* name, char type);
int getAttributeIndex(char const* name);
char const * getLookupAttributeValue(int attrIndex, int valueIndex);
int findDistanceToPolyline(int const* metr, int xc, int yc, int d, int &l);

int setviewMAP(void);
void fragment(void);
void all(void);
void centr(void);
void rasst(void);
void plmash(void);
void minmash(void);
void cor_V(void);
int drawmap(void);
void drawingline(int *metr, int num);
void draw_one_d(int x, int y, int color, int type);
void draw_one_d(double x, double y, int color, int type);
void redraw(double x1, double y1, double x2, double y2, int d);
int drawrazd(void);
int obj_enabled(F_SPR const *spr);

int printwnd(int xmin, int ymin, int xmax, int ymax, int mask1, int mask2);
void slide(void);

typedef void (*DrawCursorFunc)(const void *drawContext);

int M_K_R_status(DrawCursorFunc drawCursor = NULL, int (*keyhandler)(int) = NULL,
  const void *drawContext = NULL);

// the following functions are to be implemented within the project
int draw_one_obj(unsigned num, int color, int drawDisabled = 0);
void drawcram(void);
int poisk(int mode, int *x = NULL, int *y = NULL);

#endif