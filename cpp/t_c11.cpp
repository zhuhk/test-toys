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
#include <queue>
#include "misc.h"
#include <sparsehash/sparse_hash_map>
#include <sparsehash/dense_hash_map>
#include <stdarg.h>

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
  unordered_map<string,string> aaa;
  aaa[""] = "123";
  return;
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
void test_priority_queue(){
  priority_queue<int, vector<int>, greater<int> > q;
  q.push(10);
  q.push(11);
  q.push(12);
  q.push(13);

  cout << q.top() << endl;
  q.pop();
  cout << q.top() << endl;
  q.pop();
  cout << q.top() << endl;
  q.pop();
  cout << q.top() << endl;
  q.pop();

}
void test_heap(){
  vector<int> v;

  v.push_back(1);
  push_heap(v.begin(),v.end());
  v.push_back(7);
  push_heap(v.begin(),v.end());
  v.push_back(3);
  push_heap(v.begin(),v.end());
  v.push_back(9);
  push_heap(v.begin(),v.end());
  for (unsigned i=0; i<v.size(); i++)
    std::cout << ' ' << v[i];
  cout << endl;
}
struct SetItem{
  time_t created;
  string data;
  bool operator < (const SetItem &rhs) const{
    return created<rhs.created || (created==rhs.created && data<rhs.data);
  }
};
void test_set(){
  set<SetItem> s;
  
  SetItem item;

  for(int i = 0; i<10; i++){
    item.created = PR_Now() % 10000;
    item.data = "data" + to_string(i);
    s.insert(item);
  }
  item.data = "data" + to_string(13);
  cout << "item:" << item.created << " " << item.data << endl;
  s.insert(item);
  cout << "item:" << item.created << " " << item.data << endl;

  for(auto &i: s){
    cout << "set: " << i.created << " " << i.data << endl;
  }
  auto j = s.begin();
  cout << "first:" << j->created << " " << j->data << endl;

  cout << "item:" << item.created << " " << item.data << endl;
  auto k = s.find(item);
  if(k != s.end()){
    cout << "found " << k->created << " " << k->data << endl;
  }else{
    cout << "not found " << item.created << item.data << endl;
  }
  cout << "item:" << item.created << " " << item.data << endl;
  s.erase(item);
  k = s.find(item);
  if(k != s.end()){
    cout << "found " << k->created << " " << k->data << endl;
  }else{
    cout << "not found " << item.created << item.data << endl;
  }
}
void test_map(){
  map<string, string> m;
  m["k1"] = "v1";
  m["k2"] = "v2";
  m["k3"] = "v3";
  for(auto item: m){
    cout << "key:" << item.first << " value:" << item.second << endl;
  }
}
class C{
  public:
    ~C(){
      cout << "release C" << endl;
    }

};
void test_shared_ptr(){
  C c;
  {
    shared_ptr<C> ptr(&c);
  }
  cout << "after" << endl;
}
void test_gdb_map(){
  map<int, int> m;
  m[0] = 1;
  m[1] = 1;
  m[2] = 1;

  string str("1234");
  string str1("1234");
  return;
}

void test_map_perf1(){
  unordered_map<string, string> a1;
  a1["s1"] = "v1";
  a1["s2"] = "v2";
  vector<pair<string,string>> va1;
  for(auto &item:a1){
    va1.push_back(std::move(item));
  }

  for(auto &item:a1){
    cout << "a1:" << item.first << item.second << endl;
  }
  unordered_map<string, vector<string>> aaa({
      {"ag", {"602", "603"}},
      {"cmi", {"20736"}},
      {"crowd", {"902", "913"}},
      {"", {"501"}},
      {"rtst", {"peC5GS", "g6jY35", "Ltkoz3", "qd2CMs", "MTxQba", "4nW41X", "G7tmEX", "12BMi1", "cQoB8E", "OWz98F"}},
      {"rtuhy",{"1x118x123x126"}},
      {"rtuhy1",{"1"}},
      {"rtuhy2", {"1x118"}},
      {"rtuhy3", {"1x118x123"}},
      {"sti", {"80301", "80103"}},
      {"uch1",{"sina_home", "hdphoto", "cmnt", "sports", "kandian", "so", "sc", "news", "plan", "survey"}},
      {"uch2", {"blog_dpool", "sports_nba", "sc_news", "news_sh", "sc_null", "proc_cmnt", "finance_china", "survey_null", 
      "news_news_zt", "sports_sports_zt"}},
      {"uit", {"20736"}}
      });
  cout << "aaa:" << aaa[""][0] << endl;
  cout << "aaa:" << aaa["ag"][0] << endl;

  unordered_map<int,string> m;
  unordered_map<int,string> m1;
  unordered_map<int,string> m2;


  time_t begin;

  begin = PR_Now();
  for(int i=0; i<1000; i++){
    m[i] = "123";
    m[1000-i] = "123";
  }
  cout << "m[i]=123:" << PR_Now() - begin << endl;

  begin = PR_Now();
  for(int i=0; i<1000; i++){
    m1.insert(make_pair(i, "123"));
    m1.insert(make_pair(1000-i, "123"));
  }
  cout << "insert(make_pair):" << PR_Now() - begin << endl;

  begin = PR_Now();
  for(int i=0; i<1000; i++){
    m1.insert(std::move(make_pair(i, "123")));
    m2.insert(std::move(make_pair(1000-i, "123")));
  }
  cout << "insert(move(make_pair)):" << PR_Now() - begin << endl;

  string str1("str1"), str2("str2");

  str2 = std::move(str1);

  cout << "str1:"<< str1 << " str2:" << str2 << endl;

  char arr[1000][10];
  vector<const char*> vec;
  unordered_map<int, const char*> m3;
  for(int i=0;i<10;i++){
    vec.push_back(NULL);
    m3[i] = NULL;
  }
  const char *pch;

  begin = PR_Now();
  for(int i=1000000; i>0; i--){
    pch = m3[i%10]+1;
  }
  pch += 1;
  cout << "unordered_map:" << PR_Now() - begin << endl;

  begin = PR_Now();
  for(int i=10000000; i>0; i--){
    pch = vec[i%10]+1;
  }
  pch += 1;
  cout << "vec:" << PR_Now() - begin << endl;

  begin = PR_Now();
  for(int i=10000000; i>0; i--){
    pch = arr[i%10]+1;
  }
  pch += 1;
  cout << "arr:" << PR_Now() - begin << endl;

  vector<int> v1, v2;
  for(int i=0;i<10;i++){
    v1.push_back(i);
    v2.push_back(10-i);
  }
  v1 = std::move(v2);
  cout << v1.size() << " " << v2.size() << endl;

  map<string, vector<string>> features, newFeatures;
  features["1"].push_back("1");
  features["1"].push_back("1");
  features["1"].push_back("1");
  features["2"].push_back("2");
  features["2"].push_back("2");
  features["2"].push_back("2");

  for (const auto& feature: features) {
    vector<string> vec(std::move(feature.second));
    cout << "feature: "<< feature.first << ":" << feature.second.size() << endl;
  }

}

string f1(){
  static string str("123");
  string &str1 = str;
  return str;
}
void test_vec_perf1(){
  vector< pair<string,vector<string> > > m;
  vector< pair<string,vector<string> > > m1;
  vector< pair<string,vector<string> > > m2;
  vector< pair<string,vector<string> > > m3;
  vector< pair<string,vector<string> > > m4;

  string s1 = std::move(f1());
  string s2 = std::move(f1());
  string s3 = std::move(f1());
  string s4 = std::move(s3.substr(1,2));
  cout << "s1:" << s1 << endl;
  cout << "s2:" << s2 << endl;
  cout << "s3:" << s3 << endl;
  cout << "s4:" << s4 << endl;

  time_t begin;

  begin = PR_Now();
  string str("123");
  for(int i=0; i<10000; i++){
    pair<string, vector<string> > p(str, {"321"});
    m.push_back(p);
  }
  cout << "m.push_back:" << PR_Now() - begin << endl;

  begin = PR_Now();
  str = "3214";
  for(int i=0; i<10000; i++){
    pair<string, vector<string> > p;
    p.first = str;
    p.second.push_back("123");
    m1.push_back(p);
  }
  cout << "m1.push_back(str):" << PR_Now() - begin << endl;

  begin = PR_Now();
  str = "321";
  for(int i=0; i<10000; i++){
    pair<string, vector<string> > p;
    p.first = str;
    p.second.push_back("123");
    m2.push_back(std::move(p));
    if(i==9999){
      cout << "p:" << p.first << "=" << p.second.size() << endl;
    }
  }
  cout << "m2.push_back(move str):" << PR_Now() - begin << endl;

  begin = PR_Now();
  str = "321";
  for(int i=0; i<10000; i++){
    pair<string, vector<string> > p;
    p.first = "111";
    p.second.push_back("123");
    m4.push_back(std::move(p));
  }
  cout << "m2.push_back(move char):" << PR_Now() - begin << endl;

  begin = PR_Now();
  str = "3213";
  for(int i=0; i<10000; i++){
    pair<string, vector<string> > p;
    p.first = str;
    m3.push_back(std::move(p));
    m3.back().second.push_back("123");
  }
  cout << "m3.push_back(move back):" << PR_Now() - begin << endl;
}

void test_substr(){
  string url = "http://abc";

  auto startPos = url.find("://");
  if(startPos != string::npos){
    startPos += 3;
  } else{
    startPos = 0;
  } 
  auto slashPos = url.find("/", startPos);
  string ret = url.substr(startPos, slashPos - startPos);

  cout << ret << endl;
}

void test_refstr(){
  string str1="123", str2="456";

  const string & s = str1 + str2;

  cout << s << endl;

  printf("%p %p %p\n", &str1, &str2, &s);
}

int NumberOfSetBits(int i){
       // Java: use >>> instead of >>
       // C or C++: use uint32_t
  i = i - ((i >> 1) & 0x55555555);
  i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
  return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}

void test_bitcount(){
  time_t b,e;
  
  b = PR_Now();
  for(int i=0;i<10000;i++){
    __builtin_popcount(i);
  }
  e = PR_Now();
  cout << "builtin_popcount:" << e - b << endl;

  b = PR_Now();
  for(int i=0;i<10000;i++){
    NumberOfSetBits(i);
  }
  e = PR_Now();
  cout << "NumberOfSetBits:" << e - b << endl;

  for(int i=0;i<1000000;i++){
    int i1 = __builtin_popcount(i);
    int i2 = NumberOfSetBits(i);
    if(i1 != i2){
      cout << "diff: i1=" << i1 << " i2=" << i2 << endl;
    }
  }

}

class G{
  public: 
    int m = 0;
    string dName = "name";
};

template <class T>
class F : public G{
  T n;
};

template <class T>
class D : public F<T>{
  public:
  void print(){
    cout << "m:" << this->m << endl;
    cout << "name:" << this->dName << endl;
  }
};

void test_tpl_inherit(){
  D<int> d;

  d.print();
}
int Snprintf(char *buf, size_t buf_size, const char *fmt, ...) {
  va_list ap;
  va_start (ap, fmt);
  int real_size = vsnprintf(buf, buf_size, fmt, ap);
  va_end (ap);
  if(real_size < 0){
    real_size = 0;
  }
  if(real_size > buf_size){
    real_size = buf_size;
  }
  return real_size;
}
void test_snprintf(){
  char buf[20];
  int off = 0;

  for(int i=0; i<20; i++){
    off += Snprintf(buf+off, 9-off, "%d", i);
    printf("%d %s %d %d %d\n", off, buf, buf[9], buf[10],buf[11]);
  }
  string a;
  printf("%p %lu\n", a.c_str(), a.capacity());
  a.reserve(1024);
  printf("%p %lu\n", a.c_str(), a.capacity());
}

static void SortTopK(const unordered_map<string, float>& feature_weight,
    uint32_t topK,
    vector<string>* features) {

  if (topK >= feature_weight.size()) {
    for (const auto& it : feature_weight) {
      features->push_back(it.first);
    }
    return;
  }

  typedef std::pair<string, float> qMember;
  auto comparator = [](qMember a, qMember b){
    if(a.second == b.second){
      cout << "k:" << a.second << " " << b.second << " " << a.first << " " << b.first<< endl;
      return a.first > b.first;
    }else{
      cout << "gt:" << a.second << " " << b.second << endl;
      return a.second > b.second;
    }
  };
  priority_queue<qMember, vector<qMember>, decltype(comparator)> pqueue(comparator);

  for (const auto& it : feature_weight) {
    // sort with priority queue and select topK
    if (pqueue.size() < topK) {
      pqueue.push(it);
      cout << "push:" << it.first << endl;
    } else {
      if (comparator(it, pqueue.top())) {
        cout << "replace:" << pqueue.top().first << " with " << it.first << endl;
        pqueue.pop();
        pqueue.push(it);
      }else{
        cout << "skip:" << it.first << endl;
      }
    }
  }

  while (!pqueue.empty()) {
    features->push_back(pqueue.top().first);
    pqueue.pop();
  }
}
void t_sort(){
  unordered_map<string, float> feature_weight = { 
    {"1x244",1.0}, 
    {"2x244",10.0},
    {"3x244",11.0},
    {"4x244",8.0}
  };
  vector<string> features;
  //SortTopK(feature_weight, 2, &features);
  for(auto f: features){
    cout << "f:" << f << endl;
  }

  /*unordered_map<string, float> feature_weight1 = { 
    {"244x286x345x",1.0}, 
    {"244x286x530x",1.0},
    {"1x43x101x103",1.0},
    {"244x286x337x529",1.0},
    {"1x174x510x",1.0}
  };
  */
  unordered_map<string, float> feature_weight1 = { 
   {"21270", 0.40651}
  ,{"20951",0.06775}
  ,{"20320",0.06775}
  ,{"21275",0.06775}
  ,{"20319",0.06775}
  ,{"20318",0.06775}
  ,{"20317",0.06775}
  ,{"20634",0.06775}
  ,{"20955",0.0663}
  ,{"20216",0.05293}
  };
  features.clear();
  SortTopK(feature_weight1, 3, &features);
  for(auto f: features){
    cout << "f1:" << f << endl;
  }

}

#define add(a,b) a+b
void t_ptr(){
  int x[5]={1,2,3,4,5};
  int *ptr=(int *)(&x+1);
  cout<<*(x+1)<<*(ptr-1) << endl;
  printf("%d\n", 7*add(3,4));
  int a[] = {0,1,2,3,4,5 }, *p = a+1;

  cout << "ptr:" << endl;
  cout << *(++p) << endl;

}

class LogMessageVoidify {
   public:
       LogMessageVoidify() { }
         // This has to be an operator with a precedence lower than << but
         // higher than ?:
         void operator&(std::ostream&) { }
};

#define TEST_IF_T 1
#define TEST_IF_F 0

void t_if(){
#if TEST_IF_T
  cout << "if T is true" <<endl;
#else
  cout << "if T is false" <<endl;
#endif
#if TEST_IF_F
  cout << "if F is true" <<endl;
#else
  cout << "if F is false" <<endl;
#endif
}

class TestFunc{
  public:
    static void foo(){
      cout << 123 << endl;
    }
};

void t_map(){
  map<string, string> dict = {
    {"key","value"},
    {"key1","value3"},
    {"key1","value1"},
    {"key2","value2"}
  };

  for(auto &item: dict){
    cout << item.first << " " << item.second << endl;
  }
}

void t_move(){
  vector<string> origin = {"abc","bcd","efg"};
  vector<string> dist;
  for(auto &value: origin){
    dist.push_back(std::move(value));
  }
  cout << "origin:" << endl;
  for(auto &val: origin){
    if(val.empty()){
      continue;
    }
    cout << " " << val << endl;
  }

  cout << "dist:" << endl;
  for(auto &val: dist){
    cout << " " << val << endl;
  }

  map<string, string> m_origin{
    {"key-abc","abc"},
      {"key-bcd","bcd"},
      {"key-efg","efg"}
  };
  map<string,string> m_dist;
  for(auto &value: m_origin){
    m_dist[std::move(value.first)] = std::move(value.second);
  }
}
int main () {
  t_move();
  return 0;
  t_map();
  return 0;
  void (*ptr)() = &TestFunc::foo;

  ptr();
  return 0;

  t_if();
  return 0;
  false ? (void) 0 : LogMessageVoidify() & cout << "abc";
  return 0;
//  int i; for(i=1;i<10; i++) cout<<i<<endl;
//  int i=1; for(int j=0;i<10&&j<10; i++, j++)cout<<i+j<<endl; cout<<i;
//  int i=0; for(;i<10;i++)cout<<i;
//  for(int i=0,j=0;i<10&&j<10;i++,j++) cout<<i+j; cout<<i;
  t_ptr();
  return 0;
  t_sort();
  return 0;

  string a;
  double b = 0.1;
  a += to_string(b);
  cout << "a=" << a << endl;
  return 0;
  test_snprintf();
  return 0;
  test_tpl_inherit();
  return 0;
  test_bitcount();
  return 0;
  test_refstr();
  return 0;
  test_substr();
  return 0;
  test_map_perf1();
  return 0;
  test_vec_perf1();
  return 0;
}
