#ifndef __EXTMATH_HPP__
#define __EXTMATH_HPP__

#define min(a, b) (a)<(b)?(a):(b)
#define max(a, b) (a)>(b)?(a):(b)

long longmul(int m1, int m2);
unsigned long ulongmul(unsigned m1, unsigned m2);

double length(int deltaX, int deltaY);
double length(double deltaX, double deltaY);

int sign(int value);
int sign(double value);

#endif
