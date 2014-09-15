#include "math.h"

double cos(double x)
{
    while (x<=-PI*2)
        x+=PI*2;
    while (x>=PI*2)
        x-=PI*2;

    double val = 1;
    for (int k = TRIG_PRECISION; k >= 0; --k)
        val = 1 - x*x/(2*k+2)/(2*k+1)*val;
    return val;
}

double sin(double x)
{
    while (x<=-PI*2)
        x+=PI*2;
    while (x>=PI*2)
        x-=PI*2;

    double val = 1;
    for (int k = TRIG_PRECISION; k >= 0; --k)
        val = 1 - x*x/(2*k+2)/(2*k+3)*val;
    return x * val;
}
