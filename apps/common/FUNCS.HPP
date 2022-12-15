#ifndef __FUNCS_HPP__
#define __FUNCS_HPP__

#include <stdlib.h>

void store_otkat(void);
void restore_otkat(void);
int upcase(int symb);
void rasst(void);
void help(const char* help_file);

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

void fragment(void);
void all(void);
void centr(void);
void rasst(void);
void plmash(void);
void minmash(void);
void cor_V(void);

int M_K_R_status(void (*drawCursor)(void) = NULL, int (*keyhandler)(int) = NULL);

// the following functions are to be implemented within the project
int drawrazd(void);

#endif