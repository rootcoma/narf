#ifndef MATH_H
#define MATH_H
#define PI 3.14159
#define TRIG_PRECISION 10

#define min(a,b) a < b ? a : b
#define max(a,b) a > b ? a : b

double wrap_radians(double x);
double sin(double x);
double cos(double x);
int sqrt(int x);

#endif
