#include <iostream>       // std::cout
#include <thread>         // std::thread
#include <future>         // std::promise, std::future
#include <unistd.h>
#include <vector>

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

int main () {
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
