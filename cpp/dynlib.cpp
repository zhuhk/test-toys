#include <stdio.h>
#include <pthread.h>
#include <map>
#include <iostream>
#include <typeinfo>
#include <string>

#include "misc.h"

#include "dynlib.h"

using namespace std;

map<string, BaseResource*> resources;

#define NS_dynlib dynlib_1_1
namespace ns1{
namespace NS_dynlib{
  class Resource : public BaseResource{
    string data;
    vector<int> d;
    public:
    Resource(){
      name = __FILE__;
      NOTICE("contruct");
    }
    BaseResource* clone(){
      cout << "name:" <<  typeid(this).name() << " hash:" << typeid(this).hash_code() <<endl;
      return new Resource();
    }
    void load(){
      NOTICE("load in Resource");
      for(int i=0;i<1000000;i++){
        string str = to_string(i);
        data.append(str);
        d.push_back(i);
      }
    }
  };
}
}

extern BaseResource* same_func(){
  NOTICE("dynlib.cpp");
  return new ns1::NS_dynlib::Resource();
}
class Resource : public BaseResource{
  string data;
  vector<int> d;
  public:
  Resource(){
    name = __FILE__;
    NOTICE("contruct");
  }
  BaseResource* clone(){
    return new Resource();
  }
  void load(){
    NOTICE("load in Resource");
    for(int i=0;i<1000000;i++){
      string str = to_string(i);
      data.append(str);
      d.push_back(i);
    }
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
class Test{
  public:
 Test(){
   NOTICE("");
 }
};
void same1_func(){
  NOTICE("");
}
