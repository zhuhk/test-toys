#include "misc.h"

time_t PR_Now() {    
    struct timeval tv;    
    gettimeofday(&tv, 0);    
    return (tv.tv_sec*1000 + tv.tv_usec/1000);
}

extern const char*strtime(time_t t, char *buf, int buflen){
    struct tm tm_t;
    localtime_r(&t, &tm_t);
    strftime(buf, buflen, "%Y-%m-%d %T", &tm_t);
    return buf;
}

