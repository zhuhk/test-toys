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
  virtual BaseResource *clone() = 0;
  std::string name;
};
extern std::vector<int> vec;
extern std::map<std::string, BaseResource*> resources;

// funtions export from dynamic libs
extern "C" int dynlib_func();
extern "C" int dynlib_func2();
extern "C" int dynlib1_func1();
extern "C" int dyn_init();
extern "C" BaseResource* same_func();

// function used or defined by lib
extern int main_func();
extern int dynlib_func1();

typedef int (*func_t)();
typedef BaseResource *(*funcRes_t)();
#endif
