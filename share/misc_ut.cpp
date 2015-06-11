#include "misc.h"
#include "testharness.h"
#include <string.h>
#include <stdlib.h>

class MISC { };

TEST(MISC, time){
    time_t t1,t2;
    time(&t1);
    t2 = PR_Now();
    ASSERT_EQ(t1, t2/1000);

    int ret = setenv("TZ", "CST-8", 1);
    if(ret != 0){
      WARNING("setenv(TZ,CST-8,1)failed. ret=%d, msg:%m",ret);
    }
    ASSERT_EQ(ret, 0);

    t1 = 1391243411;
    const char *obj="2014-02-01 16:30:11";

    char buf[100]; 
    strtime(t1, buf, sizeof(buf));
    ret = strcmp(buf, obj);
    if(ret != 0){
	WARNING("ret=%d t=%lu, str:'%s'!=obj:'%s'", ret, t1, buf, obj);
	ASSERT_EQ(ret, 0);
    }
}

int main(int argc, char** argv){
    return test::RunAllTests();
}

