#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string>
#include <string.h>
#include <iostream>
#include <map>
#include <sys/time.h>
#include <math.h>
#include <vector>

using namespace std;

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
}
void t_map(){
  map<uint64_t, uint32_t> dict;

  dict[1]++;
  cout << dict[1] <<endl;
}
void t_vec(){
  vector<uint64_t> dict;
  for(int i=0; i<1000000; i++){
    dict.push_back(i);
    if(i % 10000 == 0){
      cout << &dict[0] <<endl;
    }
  }
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
  cout << "logf*100w:" << Now() - begin << "ms" <<endl;
}

void t_bit(){
  int32_t a = 3;

  char *p = (char*) &a;

  for(int i=0; i<4; i++){
    printf("%x ", p[i]);
  }
  printf("\n");
}

/*
pushq $0x3f10   immediate constant  01101000 00010000 00111111 00000000 00000000 (5)    68 10 3f 00 00
pushq %rbx  register    01010011 (1)    53
pushq (%rdx)    indirect    11111111 00110010 (2)   ff 32
pushq 0x8(%rax) indirect with displacement  11111111 01110000 00001000(3)   ff 70 08
pushq 0xff(%rbp,%rcx,4) indirect with displacement and scaled-index 11111111 01110100 10001101 11111111 (4) ff 74 8d ff
*/
const char *regNames[] = {"%rax", "%rcx", "%rdx", "%rbx", "%rsp", "%rbp", "%rsi", "%rdi"};

void print_hex(int len, const unsigned char*bytes){
  for(int i=0; i<len; i++){
    printf("%0x ", bytes[i]);
  }
}
void disassemble(const unsigned char *raw_instr){
  unsigned char opcode = raw_instr[0];
  unsigned char len = 1;

  if((opcode & 0xf8) == 0x50){ // register
    len = 1;
    unsigned char reg = opcode & 0x7;
    print_hex(len, raw_instr);
    printf("pushq %s\n", regNames[reg]);
  }else if(opcode == 0x68){ // immediate constant
    len = 5;
    print_hex(len, raw_instr);
    uint32_t *addr = (uint32_t*)(raw_instr+1);
    printf("pushq $%#x\n", *addr);
  }else if(opcode == 0xff){
    unsigned char subcode = raw_instr[1];
    if(subcode == 0x74){ // indirect with displacement and scaled-index
      len = 4;
      print_hex(len, raw_instr);
      unsigned char scaled = 1 << (raw_instr[2] >> 6);
      printf("pushq %x(%s, %s, %d)\n", raw_instr[3], regNames[(raw_instr[2]&0x3)], regNames[(raw_instr[2]>>3)&0x3], scaled);
    } else {
      switch(subcode & 0xf8){
        case 0x30: // indirect
          len = 2;
          print_hex(len, raw_instr);
          printf("pushq (%s)\n", regNames[(subcode&0x3)]);
          break;
        case 0x70: //indirect with displacement
          len = 3;
          print_hex(len, raw_instr);
          printf("pushq %#x(%s)\n", raw_instr[2], regNames[(subcode&0x3)]);
          break;
        default:
          printf("invalide code. subcode:%x\n", subcode);
      }
    }
  }else{
    printf("invalid opcode code:%x\n", opcode);
  }
}

void t_disassemble(){
  unsigned char data1[] = {0x68, 0x10, 0x3f, 0x00, 0x00};
  unsigned char data2[] = {0x53};
  unsigned char data3[] = {0xff, 0x32};
  unsigned char data4[] = {0xff, 0x70, 0x08};
  unsigned char data5[] = {0xff, 0x74, 0x8d, 0xff};

  disassemble(data1);
  disassemble(data2);
  disassemble(data3);
  disassemble(data4);
  disassemble(data5);
}
int main(){
  t_disassemble();
  return 0;
  t_bit();
  return 0;
  t_vec();
  return 0;
  t_log10f();
  return 0;
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
