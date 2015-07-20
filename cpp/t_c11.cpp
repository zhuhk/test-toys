#include <iostream>       // std::cout
#include <thread>         // std::thread
#include <future>         // std::promise, std::future
#include <unistd.h>
#include <vector>
#include <atomic>
#include <mutex>
#include "misc.h"

using namespace std;
std::promise<int> prom;

void print_global_promise () {
  std::future<int> fut = prom.get_future();
  int x = fut.get();
  std::cout << "value: " << x << '\n';
}


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
  }
}
void atomic2(uint64_t cnt){
  for(uint64_t i=0; i<cnt; i++){
    total2.fetch_add(1);
  }
}
void atomic3(uint64_t cnt){
  for(uint64_t i=0; i<cnt; i++){
    ++total3;
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
  cout << "atomic2 - time:" << PR_Now() - begin << "ms " << "total:" << total1 <<endl;

  total3 = 0;
  begin = PR_Now();
  for(int i=0; i<10; i++){
    t[i] = thread(atomic3, 1000000);
  }
  for(int i=0; i<10; i++){
    t[i].join();
  }
  cout << "atomic3 - time:" << PR_Now() - begin << "ms " << "total:" << total1 <<endl;
}
int main () {
  test_atomic();
  return 0;
  test_async();
  return 0;

  std::thread th1(print_global_promise);
  std::cout << "sleep 3s" <<endl;
  sleep(1);
  prom.set_value(10);
  th1.join();

  prom = std::promise<int>();    // prom 被move赋值为一个新的 promise 对象.

  std::thread th2 (print_global_promise);
  sleep(1);
  prom.set_value (20);
  th2.join();

  return 0;
}
