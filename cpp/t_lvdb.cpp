#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <string>
#include <iostream>
#include <algorithm>
#include "misc.h"
#include "leveldb/db.h"

using namespace std;
int main(int argc, char**argv){
    leveldb::DB* db;
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, "./testdb", &db);
    if(!status.ok()){
        WARNING("open(./testdb)failed.");
        //cerr << s.Tostring() << endl;
        goto fail;
    }
    NOTICE("open succ");
    delete db;
    return 0;
fail:
    delete db;
    return 1;
}

