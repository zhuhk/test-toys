#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <iostream>

#include "misc.h"

using namespace std;

#define TEST_PROC(v) \
    extern int test_##v(int argc, char**argv); \
    test_ ## v(argc,argv);\
    return 0;

int main(int argc, char**argv){
    TEST_PROC(a);
    TEST_PROC(time);
    return 0;
}

void test_a(int, char**){
  cout << "test" << endl;
}
void test_time(int, char**){
    time_t t1,t2;
    char strbuf[100]="123";

    t1 = PR_Now();
    for(int i=0;i<1000000;i++){ }
    t2 = PR_Now();
    fprintf(stderr, "base: t2-t1=%ldms\n", t2-t1);
    fprintf(stderr, "base: t2-t1=%ldms\n", t2-t1);
    fprintf(stderr, "base: t2-t1=%ldms\n", t2-t1);
    fprintf(stderr, "base: t2-t1=%ldms\n", t2-t1);

    t1 = PR_Now();
    for(int i=0;i<1000000;i++){
	time(NULL);
    }
    t2 = PR_Now();

    fprintf(stderr, "time: t2-t1=%ldms\n", t2-t1);

    struct timeval tv;
    t1 = PR_Now();
    for(int i=0;i<1000000;i++){
	gettimeofday(&tv, 0);    
    }
    t2 = PR_Now();
    fprintf(stderr, "gettimeofday: t2-t1=%ldms\n", t2-t1);

    time_t t = time(NULL);
    ctime_r(&t, strbuf);
    int buflen = strlen(strbuf);
    strbuf[buflen-1] = 0;
    printf("buff=%s\n",strbuf);
    fprintf(stderr, "ctime_r: %s\n", strbuf);

    struct tm tm_t;
    localtime_r(&t, &tm_t);
    asctime_r(&tm_t, strbuf);
    strbuf[strlen(strbuf)-1] = 0;
    fprintf(stderr, "asctime_r: %s\n",strbuf);

    strtime(t, strbuf,sizeof(strbuf));
    fprintf(stderr, "strtime_r: %s\n",strbuf);
    return;
}

