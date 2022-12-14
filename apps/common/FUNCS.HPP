#ifndef __FUNCS_HPP__
#define __FUNCS_HPP__

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

void all(void);
void plmash(void);
void minmash(void);
void cor_V(void);

// the following functions are to be implemented within the project
int drawrazd(void);

#endif