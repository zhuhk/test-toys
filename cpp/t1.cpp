#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <string>
#include "misc.h"

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
void t_openmp(){
  int loopCnt = 30;
  int i;
  #pragma omp parallel for 
  for(i=0;i<loopCnt;i++){
    printf("i=%03d\n", i);
  //  i++;
  }
  map<string, int> dict;
  for(int i = 0; i<1000; i++){
    dict["abc" + to_string(i)] = i;
  }
  time_t begin = PR_Now(), end;
  #pragma omp parallel for
  for(int i=0;i<10000000;i++){
    int val = 1 + i;
    loopCnt = val * 100 + i;
  }
  end = PR_Now();
  cout << "   - openmp:" << end - begin << "us" <<endl;
  begin = end;
  for(int i=0;i<10000000;i++){
    int val = 1 + i;
    loopCnt = val * 100 + i;
  }
  end = PR_Now();
  cout << "   - openmp:" << end - begin << "us" <<endl;

  begin = end;
  #pragma omp parallel for
  for(int i=0;i<1000000;i++){
    string key = "abc"+to_string(i%1000);
    loopCnt = dict[key];
  }
  end = PR_Now();
  cout << "   - openmp:" << end - begin << "us" <<endl;
  begin = end;
  for(int i=0;i<1000000;i++){
    string key = "abc"+to_string(i%1000);
    loopCnt = dict[key];
  }
  end = PR_Now();
  cout << "   - openmp:" << end - begin << "us" <<endl;

  begin = end;
  #pragma omp parallel for
  for(int i=0;i<1000;i++){
    string key = "abc"+to_string(i%1000);
    loopCnt = dict[key];
  }
  end = PR_Now();
  cout << "   - openmp:" << end - begin << "us" <<endl;
  begin = end;
  for(int i=0;i<1000;i++){
    string key = "abc"+to_string(i%1000);
    loopCnt = dict[key];
  }
  end = PR_Now();
  cout << "   - openmp:" << end - begin << "us" <<endl;
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
  symlink("./t1", "./ref.t1");
  char buf[256];
  int len = readlink("./ref.t1", buf, sizeof(buf));
  if(len>=0){
    buf[len] = 0;
  }
  printf("buf=%s.\n", buf);
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
int main(int argc, char**argv){
  t_strftime();
  return 0;
  t_openmp();
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
