#ifndef __GLOBALS_HPP__
#define __GLOBALS_HPP__

#define MAX_STORED_VIEWS 10

extern int XMAXMAPE, YMAXMAPE;
extern double XMINV, YMINV, XMAXV, YMAXV;     // габаритные координаты окна вид
extern int XMINE, YMINE, XMAXE, YMAXE;

extern double otkat[];
extern int otkatn;

double YEKRAZD(int x);
double XEKRAZD(int y);

// the following functions are to be implemented within the project
int drawrazd(void);

#endif
