#ifndef ZHUHK_MISC_H
#define ZHUHK_MISC_H

#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>


#define _LOG(SPEC,fmt, msg ...) \
    do{ \
	char _strbuf[100];\
	strtime(time(NULL), _strbuf, sizeof(_strbuf));\
	fprintf(stderr, "%s %s (%s:%d,%s:%d) " fmt "\n", SPEC, _strbuf,  __FILE__, __LINE__, __FUNCTION__, getpid(), ##msg); \
    }while(0)
#define NOTICE(fmt, msg ...) _LOG("NOTICE", fmt, ##msg)
#define WARNING(fmt, msg ...) _LOG("WARNING", fmt, ##msg)
#define DBUG(fmt, msg ...) _LOG("DBUG", fmt, ##msg)

#define TEST_PROC_TYPE(v) \
    extern int test_##v(int argc, char**argv); \
    if(strcasecmp(test_type,#v)==0){ return test_ ## v(argc,argv); }

extern time_t PR_Now();
extern const char*strtime(time_t t, char *buf, int buflen);

#endif
