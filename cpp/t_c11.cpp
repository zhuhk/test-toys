#include <iostream>       // std::cout
#include <thread>         // std::thread
#include <future>         // std::promise, std::future
#include <unistd.h>
#include <vector>
#include <atomic>
#include <mutex>
#include <set>
#include <future>
#include "misc.h"

using namespace std;

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
}

shared_future<bool> fut;
promise<bool> prom;
void fut_get(){
  bool ret = fut.get();
  NOTICE("ret=%d", ret);
  NOTICE("ret=%d", fut.get());
}
void t_future(){
  prom = promise<bool>();
  fut = prom.get_future();
  thread t1(fut_get), t2(fut_get), t3(fut_get);
  sleep(1);
  prom.set_value(false);
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

int main () {
  t_shared_ptr();
  return 0;
  test_atomic();
  return 0;
  t_vec();
  return 0;
  t_set_del();
  return 0;
  t_future();
  t_future();
  return 0;

  test_async();
  return 0;
}
