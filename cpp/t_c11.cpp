#include <iostream>       // std::cout
#include <thread>         // std::thread
#include <future>         // std::promise, std::future
#include <unistd.h>
#include <vector>
#include <atomic>
#include <cstdio>
#include <memory>
#include <mutex>
#include <set>
#include <map>
#include <unordered_map>
#include <future>
#include <functional>
#include "misc.h"
#include <sparsehash/sparse_hash_map>
#include <sparsehash/dense_hash_map>

using namespace std;
using namespace google;

#define COUT \
  cout << __FUNCTION__ << ":" << __LINE__ << " "

int async_run(int i){
  cout << "threadId " << std::this_thread::get_id() << " id" << i << endl;
  sleep(30);
  return i;
}
void test_async(){
  vector<future<int>> futs(10000);
  for(int i=0; i<10000;i++){
    // futs[i] = async(launch::async, async_run, i);
    futs[i] = async(async_run, i);

  }
  cout << "start sleep 10s" <<endl;
  sleep(10);
  cout << "wait" <<endl;
  for(int i=0; i<10000; i++){
    cout << "result " << i << " " << futs[i].get() << endl;
  }
}

uint64_t total1;
mutex mtx1;
atomic<uint64_t> total2;
atomic_long total3;
void atomic1(uint64_t cnt){
  for(uint64_t i=0; i<cnt; i++){
    ++total1;
    --total1;
  }
}
void atomic2(uint64_t cnt){
  for(uint64_t i=0; i<cnt; i++){
    total2.fetch_add(1);
    total2.fetch_sub(1);
    //total2++;
  }
}
void atomic1_load(uint64_t cnt){
  for(uint64_t i=0; i<cnt; i++){
    if(total1>0){
    }
  }
}
void atomic2_load(uint64_t cnt){
  for(uint64_t i=0; i<cnt; i++){
    if(total2>0){
    }
  }
}
void atomic3(uint64_t cnt){
  for(uint64_t i=0; i<cnt; i++){
    ++total3;
    --total3;
  }
}
void atomic1_lock(uint64_t cnt){
  for(uint64_t i=0; i<cnt; i++){
    lock_guard<mutex> lock(mtx1);
    total1++;
  }
}
void test_atomic(){
  time_t begin = 0;

  thread t[10];

  total1 = 0;
  begin = PR_Now();
  for(int i=0; i<10; i++){
    t[i] = thread(atomic1_lock, 1000000);
  }
  for(int i=0; i<10; i++){
    t[i].join();
  }
  cout << "atomic1_lock time:" << PR_Now() - begin << "ms " << "total:" << total1 <<endl;

  total1 = 0;
  begin = PR_Now();
  for(int i=0; i<10; i++){
    t[i] = thread(atomic1, 1000000);
  }
  for(int i=0; i<10; i++){
    t[i].join();
  }
  cout << "atomic1 time:" << PR_Now() - begin << "ms " << "total:" << total1 <<endl;

  total2 = 0;
  begin = PR_Now();
  for(int i=0; i<10; i++){
    t[i] = thread(atomic2, 1000000);
  }
  for(int i=0; i<10; i++){
    t[i].join();
  }
  cout << "atomic2 - time:" << PR_Now() - begin << "ms " << "total:" << total2 <<endl;

  total3 = 0;
  begin = PR_Now();
  for(int i=0; i<10; i++){
    t[i] = thread(atomic3, 1000000);
  }
  for(int i=0; i<10; i++){
    t[i].join();
  }
  cout << "atomic3 - time:" << PR_Now() - begin << "ms " << "total:" << total3 <<endl;

  total3 = 0;
  begin = PR_Now();
  for(int i=0; i<10; i++){
    t[i] = thread(atomic1_load, 1000000);
  }
  for(int i=0; i<10; i++){
    t[i].join();
  }
  cout << "atomic1_load - time:" << PR_Now() - begin << "ms " << "total:" << total3 <<endl;

  total3 = 0;
  begin = PR_Now();
  for(int i=0; i<10; i++){
    t[i] = thread(atomic2_load, 1000000);
  }
  for(int i=0; i<10; i++){
    t[i].join();
  }
  cout << "atomic2_load - time:" << PR_Now() - begin << "ms " << "total:" << total3 <<endl;
}

shared_future<bool> fut;
promise<bool> prom;
void fut_wait(){
  thread::id tid = this_thread::get_id();
  while(fut.valid()){
    fut.wait();
    bool ret = fut.get();
    COUT << tid << " ret:" << ret << endl;
  }
}
void t_future(){
  //NOTICE("fut.get");
  //fut.get();

  NOTICE("fut.wait");
  fut.wait();

  thread t1(fut_wait), t2(fut_wait), t3(fut_wait);
  while(true){
    prom = promise<bool>();
    fut = prom.get_future();
    sleep(2);
    prom.set_value(false);
    sleep(3);
  }
  t1.join();
  t2.join();
  t3.join();
}

void t_set_del(){
  set<uint64_t> values;

  for(uint64_t i=0; i<10; i++){
    values.insert(i);
  }
  for(set<uint64_t>::iterator it=values.begin(); it!=values.end();){
    printf("%lu\n", *it);
    values.erase(it++);
  }
}
void t_vec(){
  vector<int> v;
  for(int i=0; i<10; i++){
    v.push_back(i);
  }

  for(auto i : v){
    cout << i << endl;
    i = 3;
  }

  for(int i=0; i<10; i++){
    cout << "after " << v[i] <<endl;
  }

  for(auto& i : v){
    cout << i << endl;
    i = 3;
  }

  for(int i=0; i<10; i++){
    cout << "after " << v[i] <<endl;
  }
}
shared_ptr<int> g_sptr1(new int);
shared_ptr<int> g_sptrs[100];
void sptr_thread(){
  for(int i=0;i<10000000;i++){
    int k = 10;
    k = k + i;
  }
  shared_ptr<int> sptr2 = g_sptr1;
  cout << "thread:" << g_sptr1.use_count() << " " << sptr2.use_count()  << endl;
  sleep(3);
}
void sptr_perf_thread(int p){
  time_t start = PR_Now();
  int j = 0;
  int k = 0;
  for(int i=0;i<100000;i++){
   shared_ptr<int> &sptr = g_sptrs[p];
   int *ptr = sptr.get();
   //int *ptr = &k;
   for(int j = 0; j<1; j++){
     *ptr = j + i;
   }
   continue;
   //int *ptr = sptr.get();
  }
  cout << "thread perf:" << PR_Now() - start << endl;
}
void t_shared_ptr(){
  shared_ptr<int> sptr1(new int);
  *sptr1 = 1;
  shared_ptr<int> sptr2 = sptr1;
  shared_ptr<int> sptr3 = sptr2;
  sptr1.reset(new int);
  *sptr1 = 2;
  shared_ptr<int> sptr4 = sptr2;
  shared_ptr<int> sptr5 = sptr1;
  cout << sptr1.use_count() 
    << " " << sptr2.use_count()
    << " " << sptr3.use_count()
    << " " << sptr4.use_count()
    << " " << sptr5.use_count()
    << endl;
  cout << *sptr1 
    << " " << *sptr2
    << " " << *sptr3
    << " " << *sptr4
    << " " << *sptr5
    << endl;
  thread t[10];
  for(int i=0; i<sizeof(t)/sizeof(thread); i++){
    g_sptrs[i].reset(new int);
    //t[i] = thread(sptr_thread);
    t[i] = thread(sptr_perf_thread, i);
  }
  /*
  for(int i=0; i<10; i++){
    cout << g_sptr1.use_count()  << endl;
    sleep(1);
  }
  */
  for(int i=0; i<sizeof(t)/sizeof(thread); i++){
  //  cout << g_sptr1.use_count()  << endl;
    t[i].join();
  }

  for(int i=0; i<1; i++){
    t[i] = thread(sptr_perf_thread, i);
  }
  for(int i=0; i<1; i++){
    t[i].join();
  }
}
void t_hash(){
  char arr1[] = "test";
  char arr2[] = "test";
  string str1 = "test";
  string str2 = "test";
  hash<char*> arrHash;
  hash<string> strHash;
  cout << arrHash(arr1) <<endl;
  cout << arrHash(arr2) <<endl;
  cout << strHash(str1) <<endl;
  cout << strHash(str2) <<endl;
}

struct TestCopy{
  atomic<int> v1;
  TestCopy & operator=( TestCopy &input){
    v1 = input.v1.load();
    return *this;
  }
};
void t_copy(){
  TestCopy a1;
  TestCopy a2;

  a1.v1 = 100;
  a2.v1 = 2;

  a1 = a2;
  cout << "a1=" << a1.v1 << endl;
}

//typedef std::unique_ptr<std::FILE, int (*)(std::FILE *)> unique_file_ptr_t;
void test_autofile(){
  for(int i=0;i<10000;i++){
    unique_ptr<std::FILE, int (*)(std::FILE *)> uniqFile(std::fopen("test.txt", "w+"), std::fclose);
    if(!uniqFile){
      NOTICE("fail");
    }else{
      fprintf(uniqFile.get(), "%d\n", i);
    }
  }
}
void test_map_perf(){
  map<int64_t, double> dict;
  unordered_map<int64_t, float> hashDict;
  //map<int64_t, float> hashDict;
  NOTICE("sizeof(map<int64_t,double>):%lu", sizeof(map<int64_t,double>));
  NOTICE("sizeof(map<int64_t,float>):%lu", sizeof(map<int64_t,float>));
  NOTICE("sizeof(unordered_map<int64_t,float>):%lu", sizeof(unordered_map<int64_t,double>));
  NOTICE("sizeof(unordered_map<int64_t,float>):%lu", sizeof(unordered_map<int64_t,float>));
  NOTICE("sizeof(string):%lu", sizeof(string));

  NOTICE("build dict");
  for(int i=0; i<5000000; i++){
    dict[i*7+1] = i * 111.0;
  }
  sleep(2);

  NOTICE("clean dict. size:%lu", dict.size());
  //dict.clear();
  sleep(2);

  NOTICE("build hash dict");
  for(int i=0; i<5000000; i++){
    hashDict[i*7+1] = i * 113.0;
  }
  sleep(2);

  NOTICE("clean hash dict. size:%lu", hashDict.size());
  //hashDict.clear();
  sleep(2);

  sparse_hash_map<int64_t, double> smap;
  dense_hash_map<int64_t, double> dmap;
  dmap.set_empty_key(0);

  NOTICE("build sparse hash map");
  for(int i=0; i<5000000; i++){
    smap[i*7+1] = i * 111.0;
  }
  sleep(2);
  NOTICE("finish dict. size:%lu", smap.size());
  sleep(2);

  NOTICE("build dense hash map");
  for(int i=0; i<5000000; i++){
    dmap[i*7+1] = i * 111.0;
  }
  sleep(2);
  NOTICE("finish dense dict. size:%lu", dmap.size());
  sleep(2);

  time_t begin;
  for(int j=0; j<10; j++){
    begin = PR_Now();
    for(int i=0;i<10000;i++){
      dict.find(i*7+1);
    }
    NOTICE("map:%lu", PR_Now() - begin);

    begin = PR_Now();
    for(int i=0;i<10000;i++){
      hashDict.find(i*7+1);
    }
    NOTICE("unordered_map:%lu", PR_Now() - begin);

    begin = PR_Now();
    for(int i=0;i<10000;i++){
      smap.find(i*7+1);
    }
    NOTICE("sparse hash map:%lu", PR_Now() - begin);

    begin = PR_Now();
    for(int i=0;i<10000;i++){
      dmap.find(i*7+1);
    }
    NOTICE("dense hash map:%lu", PR_Now() - begin);

    begin = PR_Now();
    for(int i=0;i<10000;i++){
      dict.find(i*7+2);
    }
    NOTICE("miss map:%lu", PR_Now() - begin);

    begin = PR_Now();
    for(int i=0;i<10000;i++){
      hashDict.find(i*7+2);
    }
    NOTICE("miss unordered_map:%lu", PR_Now() - begin);

    begin = PR_Now();
    for(int i=0;i<10000;i++){
      smap.find(i*7+2);
    }
    NOTICE("miss sparse hash map:%lu", PR_Now() - begin);

    begin = PR_Now();
    for(int i=0;i<10000;i++){
      dmap.find(i*7+2);
    }
    NOTICE("miss dense hash map:%lu", PR_Now() - begin);
    NOTICE("");
  }
}
int main () {
  test_map_perf();
  return 0;
  test_autofile();
  return 0;
  t_copy();
  return 0;
  t_hash();
  return 0;
  t_shared_ptr();
  return 0;
  test_atomic();
  return 0;
  t_vec();
  return 0;
  t_set_del();
  return 0;
  t_future();
  return 0;
  test_async();
  return 0;
}
