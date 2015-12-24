#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <dlfcn.h>

#include "misc.h"

using namespace std;

#include "dynlib.h"

vector<int> vec;

int main(int, char**argv){
  vec.push_back(1);

  NOTICE("++ test for same sym which link to two so");
  func_t funcSame = NULL;
  void * handleC = dlopen("./dynlib.so", RTLD_NOW|RTLD_GLOBAL|RTLD_DEEPBIND);
  funcSame = (func_t)dlsym(handleC,"same_func");
  funcSame();

  void * handleD = dlopen("./dynlib2.so", RTLD_NOW|RTLD_GLOBAL|RTLD_DEEPBIND);
  funcSame = (func_t)dlsym(handleD,"same_func");
  funcSame();

  unlink("dynlibSym.so");
  symlink("./dynlib2.so", "dynlibSym.so");
  void * handleB = dlopen("./dynlibSym.so", RTLD_NOW|RTLD_GLOBAL|RTLD_DEEPBIND);
  if(handleB == NULL){
    NOTICE("dlopen(dynlibSym.so)failed. msg:%m");
    return 0;
  }
  funcSame = (func_t)dlsym(handleB,"same_func");
  if(funcSame == NULL){
    NOTICE("dlsym(handleB, same_func)failed. %m");
    return 0;
  }
  funcSame();

  unlink("dynlibSym.so");
  symlink("./dynlib.so", "dynlibSym.so");
  void * handleA = dlopen("./dynlibSym.so", RTLD_NOW|RTLD_GLOBAL|RTLD_DEEPBIND);
  dlclose(handleB);
  funcSame = (func_t)dlsym(handleA,"same_func");
  if(funcSame == NULL){
    NOTICE("dlsym(handleA, same_func)failed. %m");
    return 0;
  }
  funcSame();
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

int main_func(){
  NOTICE("");
  return 0;
}
