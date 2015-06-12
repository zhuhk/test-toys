#ifndef DYNLIB_H
#define DYNLIB_H
#include <vector>
#include <map>
#include <string>
#include "misc.h" 

class BaseResource{
  public:
  virtual void load(){
    NOTICE("load in BaseResource");
  }
};
extern std::vector<int> vec;
extern std::map<std::string, BaseResource*> resources;

// funtions export from dynamic libs
extern "C" int dynlib_func();
extern "C" int dynlib_func2();
extern "C" int dynlib1_func();
extern "C" int dyn_init();

// function used or defined by lib
extern void main_func();
extern void same_func();
extern int dynlib_func1();

typedef int (*func_t)();
#endif
