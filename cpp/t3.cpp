#include <iostream>       // std::cout
#include <thread>         // std::thread
#include <future>         // std::promise, std::future
#include <unistd.h>

using namespace std;
std::promise<int> prom;

void print_global_promise () {
  std::future<int> fut = prom.get_future();
  int x = fut.get();
  std::cout << "value: " << x << '\n';
}

int main () {
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
