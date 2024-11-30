#include "ocr.h"

double diff_timespec(const struct timespec *t1, const struct timespec *t0)
{
    double second = difftime(t1->tv_sec, t0->tv_sec);
    return second + ((double)t1->tv_nsec - (double)t0->tv_nsec) / 1e9;
}
