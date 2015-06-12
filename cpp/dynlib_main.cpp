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
  func_t func = (func_t)dlsym(handle,"dynlib_func");
  if(func==NULL){
    NOTICE("dlsym(handle, dynlib_func) failed. msg:%s", dlerror());
  }else{
    func();
  }

  func_t func2 = (func_t)dlsym(handle,"dynlib_func2");
  if(func2==NULL){
    NOTICE("dlsym(handle, dynlib_func2) failed. msg:%s", dlerror());
  }else{
    func2();
  }

  void * handle1 = dlopen("./dynlib1.so", RTLD_NOW);
  if(handle1 == NULL){
    NOTICE("dlopen() failed. msg:%s", dlerror());
    return -1;
  }
  func = (func_t)dlsym(handle1,"dynlib1_func");
  if(func==NULL){
    NOTICE("dlsym(handle1, dynlib1_func) failed. msg:%s", dlerror());
    goto quit;
  }
  func();
  func2();

//  for(int i=0;i<vec.size();i++){
 //   printf("%d\n",vec[i]);
  //}
  dlclose(handle);
  dlclose(handle1);
  return 0;

quit:
  if(handle){
    dlclose(handle);
  }
  if(handle1){
    dlclose(handle1);
  }
  return -1;
}

void main_func(){
  NOTICE("");
}
