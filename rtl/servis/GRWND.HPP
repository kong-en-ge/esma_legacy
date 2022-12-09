#ifndef GRWND
#define GRWND
#include <memory.h>
#include "graphics.h"

#define BORD_S 6

#define BLACK 15
#define WHITE 0

struct SVS
{
  struct viewporttype view;
  struct fillsettingstype fill;
  int color;
};

void savesettings(SVS *svs);
void restoresettings(SVS *svs);

struct REGION
{
  int left, top, right, bot;
};

int xyinregion(int x, int y, REGION *reg);
int regionregion(REGION *reg1, REGION *reg2);
int xydregion(int x, int y, int delta, REGION *reg);

struct Sreg : public REGION
{
  int rem;
  char *mem;

  Sreg(int left, int top, int right, int bot, int rem);
  ~Sreg();

  void store();
  void restore();
  void restore(int color);
};

struct GRwnd : public Sreg
{
  int nst, titlec; //номер строки,цвет фона заголовка
  char const *title; //заголовок

  GRwnd(int xl, int yt, int xr, int yb, char const *,
        int titlec_ = LIGHTBLUE, int rm = 0, int drw = 0);
  void draw(void);
  void text(const char *string, int center = 0);
  void textxy(int x, int y, char *string, int color = BLACK);
  int readst(char *, int Maxs, char *firststr = NULL, int Maxs2 = 0,
             int atrib = BLACK | (LIGHTGRAY << 4), int x_posit = 0);
  int readfloat(float *fl, float minfl, float maxfl, int MAXL,
                char *firststr = NULL, int x_posit = 0);
  int readdouble(double *fl, double minfl, double maxfl, int MAXL,
                 char *firststr = NULL, int x_posit = 0);
  void clear(void);
  void clear_str(int n);                     //очистить строку n
  void clear_str(int n, int start, int sym); // sym символов на n строке c start
};

#endif