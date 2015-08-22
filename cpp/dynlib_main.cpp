#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <dlfcn.h>

#include "misc.h"

using namespace std;

#include "dynlib.h"

vector<int> vec;

int main(int, char**argv){
  vec.push_back(1);

  void * handle = dlopen("./dynlib.so", RTLD_NOW|RTLD_GLOBAL);
 // void * handle = dlopen("./dynlib.so", RTLD_LAZY);
  if(handle == NULL){
    NOTICE("dlopen() failed. msg:%s", dlerror());
    return -1;
  }
  NOTICE("handle=%p",handle);
  func_t func = (func_t)dlsym(handle,"dynlib_func");
  if(func==NULL){
    NOTICE("dlsym(handle, dynlib_func) failed. msg:%s", dlerror());
    return 1;
  }else{
    func();
  }
  NOTICE("func=%p",func);

  func_t func2 = (func_t)dlsym(handle,"dynlib_func2");
  if(func2==NULL){
    NOTICE("dlsym(handle, dynlib_func2) failed. msg:%s", dlerror());
    return 1;
  }else{
    func2();
  }
  NOTICE("func2=%p",func2);

  void * lib1_handle1 = dlopen("./dynlib_1.so", RTLD_NOW);
  if(lib1_handle1 == NULL){
    NOTICE("dlopen() failed. msg:%s", dlerror());
    return -1;
  }
  NOTICE("lib1_handle1=%p",lib1_handle1);
  func_t lib1_func1 = (func_t)dlsym(lib1_handle1,"dynlib1_func1");
  if(lib1_func1 == NULL){
    NOTICE("dlsym(handle, dynlib1_func1) failed. msg:%s", dlerror());
    return 1;
  }
  NOTICE("func=%p", func);
  sleep(5);

  void * lib1_handle2 = dlopen("./dynlib_1.so", RTLD_NOW);
  if(lib1_handle2 == NULL){
    NOTICE("dlopen() failed. msg:%s", dlerror());
    return -1;
  }
  NOTICE("lib1_handle2=%p",lib1_handle2);

  lib1_func1();
  func();
  func2();
  func = (func_t)dlsym(lib1_handle1,"dynlib1_func");
  if(func==NULL){
    NOTICE("dlsym(lib1_handle1, dynlib1_func) failed. msg:%s", dlerror());
    goto quit;
  }
  NOTICE("func=%p", func);
  func();
  func2();

  NOTICE("close lib1_handle1");
  dlclose(lib1_handle1);
  func();
  func2();

//  for(int i=0;i<vec.size();i++){
 //   printf("%d\n",vec[i]);
  //}
  dlclose(lib1_handle2);
  dlclose(handle);
  return 0;

quit:
  if(handle){
    dlclose(handle);
  }
  if(lib1_handle1){
    dlclose(lib1_handle1);
  }
  return -1;
}

void main_func(){
  NOTICE("");
}
