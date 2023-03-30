#ifndef __FUNCS_HPP__
#define __FUNCS_HPP__

#include <stdlib.h>

#include "txtpacks.hpp"
#include "fastmod.hpp"
#include "viewctx.hpp"

void store_otkat(void);
void restore_otkat(CViewContext const& viewContext);
int upcase(int symb);
void rasst(CViewContext const& viewContext);
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

typedef int bool;
#define false 0
#define true  1

class CSearchResultCollector
{
private:
  int _maxCount;
  int _count;
  OBJND* obj;
public:
  CSearchResultCollector(int maxCount);
  ~CSearchResultCollector(void);

  bool addObject(int index, double distance, int color);
  int getCount(void) const
  {
    return _count;
  }
  OBJND const* getObj(void) const
  {
    return obj;
  }
  bool isFull(void) const;
  double maxDistance(void) const;
};

int selectObjectPoint(double *x, double *y);
int chooseObject(CViewContext const& viewContext,
  const OBJND *obj, int nobj);
void whatisit(CViewContext const& viewContext);
void displayObjectInfo(int objIndex);
int ATR(TEXT_PACK_MENUS *tp, int obj);

int checkAttributeName(int index, char const* name);
int checkAttributeDef(int index, char const* name, char type);
int getAttributeIndex(char const* name);
char const * getLookupAttributeValue(int attrIndex, int valueIndex);
int findDistanceToPolyline(int const* metr, int xc, int yc, int d, int &l);

int setviewMAP(CViewContext const * pViewContext);
void fragment(CViewContext const& viewContext);
void all(CViewContext const& viewContext);
void centr(CViewContext const& viewContext);
void rasst(void);
void plmash(CViewContext const& viewContext);
void minmash(CViewContext const& viewContext);
void drawingline(int *metr, int num);
void draw_one_d(int x, int y, int color, int type);
void draw_one_d(double x, double y, int color, int type);
void redraw(CViewContext const& viewContext,
  double x1, double y1, double x2, double y2, int d);
int drawrazd(CViewContext const& viewContext);
int obj_enabled(CViewContext const& viewContext, F_SPR const *spr);

int printwnd(int xmin, int ymin, int xmax, int ymax, int mask1, int mask2);
void slide(void);

typedef void (*DrawCursorFunc)(const void *drawContext);

int M_K_R_status(DrawCursorFunc drawCursor = NULL, int (*keyhandler)(int) = NULL,
  const void *drawContext = NULL);

// the following functions are to be implemented within the project
void drawcram(void);
int poisk(CViewContext const& viewContext, int mode, int *x = NULL, int *y = NULL);

#endif