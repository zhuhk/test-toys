
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <execinfo.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <string>
#include <iostream>
#include <algorithm>
#include "misc.h"

#define TEST(func) \
      void ut_ ## func();\
  cout << "## ut_" << #func << endl;\
  ut_ ## func();\
  cout << endl

using namespace std;

int main(int argc, char**argv){
    TEST(map);
    TEST(int);
    TEST(mul);
    TEST(string);
    //TEST(strtoul);
    //TEST(time);
    return 0;
}
void ut_map(){
  pair<string, vector<string> > p("k1", {"v1"});
}
void ut_int(){
  int32_t v = -5;
  int32_t v1 = v % 3;
  cout << "-5 % 3:" << v1 <<endl;
  
  v = 5;
  v1 = v % 3;
  cout << "5 % 3:" << v1 <<endl;
}
void ut_mul(){
    int v1 = 3000;
    float v2 = 0.35;

    printf("%d,%f\n", (int)(v1*v2), v1*v2);
    v1 *= v2;
    printf("%d\n",v1);
    int32_t cnt = 1;
    uint32_t ucnt = 1;
    v2 = 1 - (cnt -2)*0.05;
    printf("v2(int32):%f, v2(uint32):%f\n",v2, 1-(ucnt-2)*0.1);
    v1 = 3000;
    v1 *= v2;
    printf("v1:%d\n",v1);
}
void ut_strtoul(){
    string str;

    str = "-1";
    uint64_t val = strtoul(str.c_str(), NULL, 10);
    cout << "val:" << val <<endl;
}
void ut_stream(){
    string line;
    while(cin.good() && getline(cin, line, '\r')){
        cout << "line='" << line << "'" <<endl;
    }
}
void ut_ctype(){
    char ch;

    ch = 255;
    if(isspace(ch)){
        printf("isSpace\n");
    }
    ch = -1;
    if(isspace(ch)){
        printf("isSpace\n");
    }
    ch = 0x81;
    if(isspace(ch)){
        printf("isSpace\n");
    }
    ch = 130;
    if(isspace(ch)){
        printf("isSpace\n");
    }

}
void ut_transfrom(){
    std::string s="hello";
    std::string out;
    std::transform(s.begin(), s.end(), out.begin(), ::toupper);
    std::cout << "hello in upper case: " << out << std::endl;
}
void ut_enum() {
    enum test{
        v1,
        v2,
        v3,
    };
}
void ut_string(){
    using namespace std;

    string s1 = "abc123";
    s1 = s1.substr(0,3);
    cout << s1 <<endl;

    NOTICE("sizeof(\"abc\")=%lu", sizeof("abc"));
    string s = "123456";
    cout << "(123456).find(45)" "pos=" << s.find("45") << endl;
    cout << "(123456).find(123)" "pos=" << s.find("123") << endl;

    cout << "(123456).erase(0,3):" << s.erase(0,sizeof("123")-1) <<endl;
    NOTICE("sizeof(string)=%lu", sizeof(string));
    NOTICE("sizeof(void *)=%lu", sizeof(void *));
    string str1;
    string str = str1;
    printf("%s\n", str.c_str());
    printf("%d\n", str.c_str()[0]);
    if(strcasecmp(str.c_str(),"123")){
    }
    NOTICE("sizeof(void *)=%p", str.c_str());
    str = "123";
    NOTICE("sizeof(void *)=%p", str.c_str());
    str.clear();
    NOTICE("sizeof(void *)=%p", str.c_str());
}
void ut_time(){
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

