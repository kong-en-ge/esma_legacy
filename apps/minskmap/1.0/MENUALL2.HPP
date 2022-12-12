#ifndef menuall2
#define menuall2

#include "dbf.hpp"
#include "mouse.hpp"
#include "button.hpp"
#include "drawrazd.hpp"
#include "globalda.hpp"
#include "textpack.hpp"
#include "set.hpp"
#include "bigpath.hpp"

void fragment(void);
void all(void);
void centr(void);
void rasst(void);
void slide(void);
void find_rg(void);
void find_st(void);
int find_bl(char *parst = NULL, int parbl = 0);
void onoff(void);
void chmark(void);
void whatisitd(int mode);
void finddbf(void);
void xycoord(void);
void polygon(int);
void pascal clearm(int);
double YEKRAZD(int x);
double XEKRAZD(int y);
int XRAZEKR(float y);
int YRAZEKR(float x);
double YDISRAZ(int x);
double XDISRAZ(int y);

#endif