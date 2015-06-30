#include <iostream>       // std::COUT
#include <thread>         // std::thread
#include <future>         // std::promise, std::future
#include <unistd.h>
#include <memory>

using namespace std;

#define COUT cout << __FILE__ << ":" << __LINE__ << "-" << __FUNCTION__ << "\t"

class Base{
  public:
    Base(){
      COUT << "construct Base id:" << id_ << endl;
    }
    virtual void run(){
      COUT << "run in base id:" << id_ << endl;
    }

    ~Base(){
      COUT << "destruct Base id:" << id_ << endl;
    }
    int id_;
};

class Sub:public Base{
  public:
    Sub(int id){
      id_ = id;
      COUT << "construct Sub id:" << id_ << endl;
    }
    virtual void run(){
      COUT << "run in sub id:" << id_ << endl;
    }

    ~Sub(){
      COUT << "destruct Sub id:" << id_ << endl;
    }
};
static shared_ptr<Base> bases[2];

void Ref(int id){
  shared_ptr<Sub> sub(new Sub(id));
  COUT << "use count:" << bases[0].use_count() << endl;
  bases[0] = sub;
  COUT << "use count:" << bases[0].use_count() << endl;
}
int main () {
  Ref(1);
  shared_ptr<Sub> p1 = static_pointer_cast<Sub>(bases[0]);
  COUT << "use count:" << bases[0].use_count() << endl;
  Ref(2);
  COUT << "use count:" << bases[0].use_count() << endl;
  return 0;
}
