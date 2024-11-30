#ifndef LOG_LEVEL
#define LOG_LEVEL 1
#endif

#ifndef LOG_SEPARATOR
#define LOG_SEPARATOR "-------------------------------------------"
#endif

#define _POSIX_C_SOURCE 199309L

#include <time.h>
double diff_timespec(const struct timespec *t1, const struct timespec *t0);
