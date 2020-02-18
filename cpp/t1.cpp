#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <iostream>
#include <map>
#include <set>
#include <unordered_set>
#include <vector>
#include <string>
#include "misc.h"
#include <pthread.h>
#include <omp.h> 
#include <future>

using namespace std;
void func(int cnt){
  float a = 1.1 , b=1.2;

  for(int i=0;i<cnt;i++){
    a *= b;
  }
}
void diff(int loopCnt, int cnt){
  time_t begin = PR_Now(), end;

  cout << " + loopCnt=" << loopCnt << " cnt=" << cnt <<endl;
  for(int i=0;i<loopCnt;i++){
    func(cnt);
  }

  end = PR_Now();
  cout << "   - serial:" << end - begin << "us" <<endl;
  
  begin = end;
  #pragma omp parallel for
  for(int i=0;i<loopCnt;i++){
    func(cnt);
  }
  end = PR_Now();
  cout << "   - openmp:" << end - begin << "us" <<endl;

}
void t_mopenmp(){
  int loopCnt = 10;
  omp_set_nested(1);
  printf("main:%d\n",pthread_self());
  #pragma omp parallel for num_threads(2)
  for(int i=0;i<loopCnt;i++){
    printf("i:%d thread:%d\n",i, pthread_self());
    #pragma omp parallel for num_threads(3)
    for(int j=0;j<loopCnt;j++){
      printf("i:%d j:%d thread:%d\n",i, j, pthread_self());
      sleep(5);
    }
    sleep(5);
  }
}
void t_openmp(int threads){
  int loopCnt = 30;
  int i;
  #pragma omp parallel for num_threads(threads)
  for(i=0;i<loopCnt;i++){
  //  printf("i=%03d\n", i);
  //  i++;
  }
  map<string, int> dict;
  for(int i = 0; i<1000; i++){
    dict["abc" + to_string(i)] = i;
  }
  time_t begin = PR_Now(), end;
  #pragma omp parallel for num_threads(threads)
  for(int i=0;i<100000000;i++){
    int val = 1 + i;
    loopCnt = val * 100 + i;
  }
  end = PR_Now();
  cout << "- openmp-1000w:" << end - begin << "us" <<endl;
  begin = PR_Now();
  for(int i=0;i<100000000;i++){
    int val = 1 + i;
    loopCnt = val * 100 + i;
  }
  end = PR_Now();
  cout << "- normal-1000w:" << end - begin << "us" <<endl;

  begin = PR_Now();
  #pragma omp parallel for num_threads(threads)
  for(int i=0;i<1000000;i++){
    string key = "abc"+to_string(i%1000);
    loopCnt = dict[key];
  }
  end = PR_Now();
  cout << "- openmp-dict-100w:" << end - begin << "us" <<endl;
  begin = PR_Now();
  for(int i=0;i<1000000;i++){
    string key = "abc"+to_string(i%1000);
    loopCnt = dict[key];
  }
  end = PR_Now();
  cout << "- normal-dict-100w:" << end - begin << "us" <<endl;

  begin = end;
  #pragma omp parallel for num_threads(threads)
  for(int i=0;i<1000;i++){
    string key = "abc"+to_string(i%1000);
    loopCnt = dict[key];
  }
  end = PR_Now();
  cout << "- openmp-dict-1k:" << end - begin << "us" <<endl;
  begin = end;
  for(int i=0;i<1000;i++){
    string key = "abc"+to_string(i%1000);
    loopCnt = dict[key];
  }
  end = PR_Now();
  cout << "- normal-dict-1k:" << end - begin << "us" <<endl;
}

void t_set(){
  set<uint64_t> list;
  for(int i=0; i<20; i++){
    list.insert(random());
  }
  set<uint64_t>::iterator it;
  for(it = list.begin(); it != list.end(); it++){
    cout << *it << endl;
  }
}

void t_readlink(){
  int ret = symlink("./t1", "./ref.t1");
  char buf[256];
  int len = readlink("./ref.t1", buf, sizeof(buf));
  if(len>=0){
    buf[len] = 0;
  }
  printf("buf=%s.\n", buf);
  return;
}

void t_it(){
  vector<int*> vec;
  vector<int*> vec1;
  int data[100];
  for(int i=0; i<100; i++){
    data[i] = i;
    vec.push_back(data+i);
  }
  for(vector<int*>::iterator it = vec.begin(); it!=vec.end(); it++){
    vec1.push_back(*it);
  }
  for(vector<int*>::iterator it = vec1.begin(); it!=vec1.end(); it++){
    printf("%d\n", **it);
  }
}
void t_strftime(){
  time_t begin = PR_Now();
  for(int i=0; i<100000; i++){
    char _strbuf[100];
    strtime(time(NULL), _strbuf, sizeof(_strbuf));
  }
  cout << "time:" << PR_Now()-begin <<endl;
}
void t_localtime_r(){
  time_t begin = PR_Now();
  for(int i=0; i<100000; i++){
    char _strbuf[100];
    time_t now = time(NULL);
    struct tm result;
    localtime_r(&now,  &result);
  }
  cout << "time:" << PR_Now()-begin <<endl;
  begin = PR_Now();
  for(int i=0; i<100000; i++){
    char _strbuf[100];
    time_t now = time(NULL);
    struct tm result;
    gmtime_r(&now,  &result);
  }
  cout << "time:" << PR_Now()-begin <<endl;
  time_t now = time(NULL);
  struct tm result;
  localtime_r(&now,  &result);
  printf("%d%02d%02d %02d%02d%02d\n",
      1900+result.tm_year, result.tm_mon, result.tm_mday, result.tm_hour, result.tm_min, result.tm_sec);
  now += 8*3600;
  gmtime_r(&now,  &result);
  printf("%d%02d%02d %02d%02d%02d\n",
      1900+result.tm_year, result.tm_mon, result.tm_mday, result.tm_hour, result.tm_min, result.tm_sec);
}

class cls{
  public:
  const static string abc;
};
const string cls::abc = "123";

struct ReqDist{
  vector<double> real_reqs;
};


void t_vec(){
  vector<double> vec(24,0.1);
  vector<double> hourly_rates[2]; // 周期性更新
  vector<double> hourly_reqs(24, 0.0); // 周期性更新，根据reql_reqs估算当前小时的计数
  vector<double> real_reqs(24,0.0);
  for(auto &item: vec){
    cout << " " << item;
    item++;
  }
  cout << endl;

  for(auto &item: vec){
    cout << " " << item;
    item++;
  }
  cout << endl;

  vector<double> vec1(24,0.1);
  vec1 = vec;
  for(auto &item: vec1){
    cout << " " << item;
  }
  cout << endl;

  vector<double> v1(24);
  vector<double> v2(2);
  v1 = v2;
  cout << v1.size() << " " << v2.size() << endl;

  vector<double> v3[2];
}

string str_tm(struct tm &t){
  char buf[30];
  snprintf(buf, sizeof(buf), "%02d-%02d-%02d %02d:%02d:%02d",
      t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);
  return buf;
}
void t_time(){
  struct tm tm1, tm2;

  time_t now;
  time(&now);

  now += 3600 * 8;
  
  gmtime_r(&now, &tm1);

  tm2 = tm1;
  tm2.tm_min = 59;
  tm2.tm_sec = 59;
  //time_t t1 = mktime(&tm2);
 
  time_t t1 = now + (59-tm1.tm_min) * 60 + (59-tm1.tm_sec);

  cout << "t:" << str_tm(tm1) << " t1:" << str_tm(tm2) << endl;
  cout << now << " " << t1 << " " << t1 - now << endl;
}
void t_map(){
  unordered_set<int> s{2,3};
  s = {1,2};
  for(auto &item: s){
    cout << " " << item;
  }
  cout << endl;
}
void t_prom(){
  promise<bool> prom;

//  future<bool> fut = prom.get_future();

  prom.set_value(true);

  future<bool> fut1 = prom.get_future();
//  prom.set_value(true); // failure
}

class A{
public:
  A(string str){
    cout << "construct A msg:" << str << endl;
  }
  ~A(){}
};
void t_local_var(){
  cout << "begin" << endl;

  A a("before");
  return;

  A b("after");
}
int main(int argc, char**argv){
  t_local_var();
  return 0;
  t_prom();
  return 0;
  t_map();
  return 0;
  t_vec();
  return 0;
  t_time();
  return 0;
  for(int i=1;i<20;i++){
    printf("++ threads:%d\n",i);
    t_openmp(i);
  }
  return 0;
  t_mopenmp();
  return 0;
  cout << cls::abc <<endl;
  return 0;
  t_strftime();
  return 0;
  t_localtime_r();
  return 0;
  t_it();
  return 0;
  t_readlink();
  return 0;
  t_set();
  return 0;
  diff(1,10000);
  diff(1,1000000);
  diff(10,10000);
  diff(10,1000000);
  diff(1000000,10);
  diff(10000,10000);

  return 0;
}
