#ifndef LOG_LEVEL
#define LOG_LEVEL 1
#endif

#ifndef LOG_SEPARATOR
#define LOG_SEPARATOR "-------------------------------------------"
#endif


#include <time.h>
double diff_timespec(const struct timespec *t1, const struct timespec *t0);
