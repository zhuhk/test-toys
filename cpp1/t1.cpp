#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string>
#include <string.h>
#include <iostream>
#include <map>
#include <sys/time.h>
#include <math.h>

using namespace std;

#define INFO(fmt,msg ...) \
  do{ \
    fprintf(stderr, "%s %s %d " fmt "\n", __FILE__ ":" , __FUNCTION__, __LINE__, ##msg); \
  }while(0)

string get_object_id(string &str){
  size_t off = str.find("object_id=");
  if(off != string::npos){
    off += sizeof("object_id=") - 1;
    size_t end = str.find("&",off);
    if(end == string::npos){
      return str.substr(off);
    } else {
      return str.substr(off, end-off);
    }
  }
  return "";
}
void t_string(){
  string str = "&object_id=123";
  string object_id = get_object_id(str);
  if(object_id == "123"){
    cout << "pass - str:" << str << endl;
  } else {
    cout << "fail - str:" << str << endl;
  }

  str = "&object_id=123&s=1";
  object_id = get_object_id(str);
  if(object_id == "123"){
    cout << "pass - str:" << str << endl;
  } else {
    cout << "fail - str:" << str << endl;
  }

  str = "source=423&object_id=123&s=1";
  object_id = get_object_id(str);
  if(object_id == "123"){
    cout << "pass - str:" << str << endl;
  } else {
    cout << "fail - str:" << str << endl;
  }

  str = "{\"id\":1412308022239837,\"text\":{\"type\",";
  size_t id_len = str.find(",");
  if(id_len!=string::npos){
    id_len -= sizeof("{\"id\":")-1;
  }
  string id = str.substr(sizeof("{\"id\":")-1, id_len);
  if(id == "1412308022239837"){
    cout << "pass - str:" << str << endl;
  } else {
    cout << "fail - str:" << str << endl;
  }
}
void t_map(){
  map<uint64_t, uint32_t> dict;

  dict[1]++;
  cout << dict[1] <<endl;
}

void t_memmove(){
  char buff[10];

  for(int i=0;i<10;i++){
    buff[i] = i;
  }

  memmove(buff+3, buff, 5);

  for(int i=0;i<10;i++){
    printf("i=%d %d\n",i, buff[i]);
  }
}

class GrandParent{
public:
  virtual int func(){
    cout << "in GrandParent" << " " << __FUNCTION__ << endl;
    return 0;
  }
  virtual int func1(){
    cout << "in GrandParent" << " " << __FUNCTION__ << endl;
    return 0;
  }
};
class Parent:public GrandParent{
public:
  int func(){
    cout << "in Parent" << " " << __FUNCTION__ << endl;
    return 0;
  }
  int func1(){
    cout << "in Parent" << " " << __FUNCTION__ << endl;
    return 0;
  }
  virtual int func2(){
    cout << "in Parent" << " " << __FUNCTION__ << endl;
    return 0;
  }
  int func3(){
    cout << "in Parent" << " " << __FUNCTION__ << endl;
    return 0;
  }
};

class Children:public Parent{
public:
  int func(){
    cout << "in Children" << " " << __FUNCTION__ << endl;
    return 0;
  }
  int func1(){
    cout << "in Children" << " " << __FUNCTION__ << endl;
    return 0;
  }
  int func2(){
    cout << "in Children" << " " << __FUNCTION__ << endl;
    return 0;
  }
  virtual int func3(){
    cout << "in Children" << " " << __FUNCTION__ << endl;
    return 0;
  }
};

void t_virtualFunc(){
  Children children;
  GrandParent *gp = &children;
  Parent *p = &children;
  gp->func();
  gp->func1();
  p->func();
  p->func1();
  p->func2();
  p->func3();
}

class Any{
  public:
  Any(){
    cout << "in Constructor" <<endl;
  }

  ~Any(){
    cout << "in deConstructor" <<endl;
  }
};

Any test_any(){
  return Any();
}
void t_any(){
  test_any();
}

time_t Now(){
  struct timeval start;
  gettimeofday(&start,NULL);
  return start.tv_sec*1000+ start.tv_usec/1000;
}
void t_mktime(){
  struct tm time_tm;

  time_t begin;
  begin = Now();
  for(int i=0;i<1000000;i++){
    time(NULL);
  }
  cout << "time*100W:" << Now() - begin << "ms" <<endl;

  begin = Now();
  for(int i=0;i<1000000;i++){
    timegm(&time_tm);
  }
  cout << "timegm*100w:" << Now() - begin << "ms" <<endl;

  begin = Now();
  for(int i=0;i<1000000;i++){
    timelocal(&time_tm);
  }
  cout << "timelocal*100w:" << Now() - begin << "ms" <<endl;

  begin = Now();
  for(int i=0;i<1000000;i++){
    mktime(&time_tm);
  }
  cout << "mktime*100w:" << Now() - begin << "ms" <<endl;
}

void t_log10f(){
  time_t begin;

  begin = Now();
  for(int i=0;i<100000;i++){
    float x = 1.0*i;
    log10f(x);
  }
  cout << "logf*10w:" << Now() - begin << "ms" <<endl;
}

void t_setenv(){
  time_t begin;

  begin = Now();
  for(int i=0;i<100000;i++){
    setenv("TZ", "CST-8", 1);
  }
  cout << "setenv*10w:" << Now() - begin << "ms" <<endl;
}

void t_1(){
  char a1 = 7;
  int a[6]={1,2,3,4,5,6};
  char a2 = 8;
  int *ptr=(int *)(&a+1);
  printf("%p %p %p %p %p\n",a, &a, a+1, &a1, &a2);
  printf("%p %d\n",ptr,*(ptr));
  ptr=(int *)(a+1);
  printf("%p %d\n",ptr,*(ptr));

}
int main(){
  int data = '\0';
  cout << data <<endl;
  return 0;
  t_log10f();
  return 0;

  t_1();
  return 0;
  t_string();

  return 0;

  t_setenv();


  t_log10f();
  t_mktime();
  return 0;
  t_any();
  return 0;

  t_virtualFunc();
  return 0;
  t_memmove();
  return 0;
  t_map();
  return 0;

  t_string();

  return 0;
  uint32_t a=1000;

  a *= 0.95;

  printf("%u\n",a);
  return 0;
}
