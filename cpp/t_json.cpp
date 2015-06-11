#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <string>
#include <iostream>
#include <algorithm>
#include <fstream>

#include "misc.h"
#include "json/json.h"

using namespace Json;
using namespace std;

int main(int argc, char**argv){
    fstream ifs;
    if(argc>1){
        ifs.open(argv[1]);
    }else{
        ifs.open("test.json");
    }

    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(ifs, root, false)){
        return -1;
    }

    string name;
    Value val = root["name"];
    if(val.isString()){
        name = val.asString();
        cout << "isString" << endl;
    }
    val = root["render_data"];
    if(val.isString()){
        cout << "render_data:" << val.asString() << endl;
    }
    int age = root["age"].asInt();

    std::cout << true <<endl;
    std::cout << name << std::endl;
    std::cout << age << std::endl;

    Json::FastWriter writer;
    string result = writer.write(root);
    //cout << "json='" << result << "'" <<endl;

    return 0;return 0;
}
