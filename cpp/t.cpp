#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <string>
#include <iostream>

#include "misc.h"

void t_time();
void t_string();
void t_enum();

int main(int argc, char**argv){
	t_string();
//    t_time();
    return 0;
}
void t_enum() {
	enum test{
		v1,
		v2,
		v3,
	};
}
void t_string(){
	using namespace std;
	
	NOTICE("sizeof(string)=%lu", sizeof(string));
	NOTICE("sizeof(void *)=%lu", sizeof(void *));

	static union {
		void * align;
		char s[sizeof(string)];
	} s[2];

	string* const no = new(s[0].s) string("123457891111111111");
	cout << *no <<endl;

	char str[10]="aaaaa";
	string *pstr = new(str) string("12345");
	cout << *pstr <<endl;
	NOTICE("str(p)=%p,str=%s\n", &str, str);
	NOTICE("pstr  =%p\n", pstr);
}
void t_time(){
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

