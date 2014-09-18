#include "math.h"


double wrap_radians(double x)
{
    while (x<-PI*2)
        x+=PI*2;
    while (x>PI*2)
        x-=PI*2;
    return x;
}

double cos(double x)
{
    x = wrap_radians(x);
    double val = 1;
    for (int k = TRIG_PRECISION; k >= 0; --k)
        val = 1 - x*x/(2*k+2)/(2*k+1)*val;
    return val;
}

double sin(double x)
{
    x = wrap_radians(x);
    double val = 1;
    for (int k = TRIG_PRECISION; k >= 0; --k)
        val = 1 - x*x/(2*k+2)/(2*k+3)*val;
    return x * val;
}

int sqrt(int x) {
    long long l = 0, r = x;
    while (l <= r) {
        long long mid = l + (r - l) / 2; // (l + r) / 2;
        long long midmid = mid * mid;
        // check if x falls into [mid^2, (mid + 1)^2) 
        if ((midmid <= x) && (x < (mid + 1) * (mid + 1))) return mid;             if (midmid > x) {
            r = mid - 1;
        } else {
            l = mid + 1;
        }
    }
}