
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <string>
#include <iostream>
#include <algorithm>
#include "misc.h"

#define TEST(func) \
      void ut_ ## func();\
  cout << "## ut_" << #func << endl;\
  ut_ ## func();\
  cout << endl

using namespace std;
int main(int argc, char**argv){
    TEST(obj);
    return 0;
}

#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h" 
class Obj{
  public:
  int i;
  int j;
  Obj(){
    i = 1;
    j = 2;
  }

  template <typename Writer>
    void Serialize(Writer& writer) const {
      writer.StartObject();
      writer.String("i");
      writer.Int(i);

      writer.String("key");
      writer.String("value");
      writer.String("k\tey");
      writer.String("va\"lu\"\ne");
      writer.String("j");
      writer.Int(j);
      writer.EndObject();
    }

};
void ut_obj(){
  Obj obj;
  rapidjson::StringBuffer sb;
  rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
  obj.Serialize(writer);
  cout << "Writer:\n"<< sb.GetString() << endl;
  cout << endl;

  sb.Clear();
  rapidjson::PrettyWriter<rapidjson::StringBuffer> prettyWriter(sb);
  obj.Serialize(prettyWriter);
  cout << "PrettyWriter:\n"<< sb.GetString() << endl;
  cout << endl;
}

