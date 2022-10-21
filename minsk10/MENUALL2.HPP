#ifndef menuall2
#define menuall2
#include "dbf.hpp"
#include "library.hpp"
#include "mouse.hpp"
#include "button.hpp"
#include "drawrazd.hpp"
#include "menuall.hpp"
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
int find_bl(char* parst=NULL,int parbl=0);
void onoff(void);
void chmark(void);
void whatisitd(int mode);
void finddbf(void);
void xycoord(void);
void polygon(int);
void pascal clearm(int);
double pascal YEKRAZD(int x);
double pascal XEKRAZD(int y);
int pascal XRAZEKR(float y);
int pascal YRAZEKR(float x);
double pascal YDISRAZ(int x);
double pascal XDISRAZ(int y);
void predproc(int num);
void errorproc(int num);
#endif