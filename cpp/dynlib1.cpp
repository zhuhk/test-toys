#include <stdio.h>
#include "misc.h"

#include "dynlib.h"

class Resource : public BaseResource{
  void load(){
    NOTICE("load in Resource");
  }
};

int dynlib1_func(){
  NOTICE("from dynlib1");
  dynlib_func();
  vec.push_back(3);
  NOTICE("call dynlib_func1");
  dynlib_func1();

  NOTICE("call main_func()");
  main_func();

  NOTICE("call same_func()");
  same_func();

  Resource *res = new Resource;
  resources["dynlib1"] = res;

  res = new Resource;
  resources["dynlib2"] = res;
  return 0;
}
void same_func(){
  NOTICE("");
}
int dynlib1_func1(){
  NOTICE("from dynlib1");
  return 0;
}
