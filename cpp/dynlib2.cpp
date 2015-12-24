#include <stdio.h>
#include <pthread.h>
#include <map>

#include "misc.h"

#include "dynlib.h"

using namespace std;

map<string, BaseResource*> resources;

class Test{
  public:
 Test(){
   NOTICE("");
 }
};
int dynlib_func(){
  NOTICE("from dynlib_func()");
  NOTICE("call main_func()");
  main_func();

  NOTICE("call same_func()");
  same_func();

  vec.push_back(2);
  return 0;
}
int dynlib_func1(){
  NOTICE("from dynlib");

  NOTICE("call same_func()");
  same_func();

  vec.push_back(2);
  return 0;
}
int dynlib_func2(){
  NOTICE("resources.size()=%lu", resources.size());
  map<string, BaseResource*>::iterator it;
  for(it=resources.begin(); it!=resources.end(); it++){
    NOTICE("name:%s", it->first.c_str());
    it->second->load();
  }
  return 0;
}
extern int same_func(){
  Test * t = new Test();
  NOTICE("");
  return 0;
}
void same1_func(){
  NOTICE("");
}
