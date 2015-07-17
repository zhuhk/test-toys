/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include <unistd.h>
#include <iostream>

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

#include "./gen-cpp/Calculator.h"

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using namespace tutorial;
//using namespace shared;

time_t PR_Now() {
  struct timeval tv;
  gettimeofday(&tv, 0);
  return (tv.tv_sec*1000 + tv.tv_usec/1000);
}

#define COUT cout << __LINE__ << " - " << id << " "
int main(int argc, char** argv) {
  uint32_t id = 0;

  if(argc > 1){
    id = atoi(argv[1]);
  }
  boost::shared_ptr<TSocket> socket(new TSocket("localhost", 9090));
  boost::shared_ptr<TFramedTransport> transport(new TFramedTransport(socket));
  //boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
  boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
  CalculatorClient client(protocol);

  try {
    transport->open();
    string value;
    client.getName(value);
    cout <<  "getName:" << value <<endl;

    client.getVersion(value);
    cout <<  "getVersion:" << value <<endl;

    cout <<  "getStatus:" << client.getStatus() <<endl;

    client.getStatusDetails(value);
    cout <<  "getStatusDetails:" << value <<endl;

    client.getCpuProfile(value, 10);
    cout <<  "getCpuProfile:" << value <<endl;

    cout <<  "aliveSince:" << client.aliveSince() <<endl;

    map<string, int64_t> map_value;
    client.getCounters(map_value);
    cout <<  "getCounters:" <<endl;
    for(map<string, int64_t>::iterator it=map_value.begin(); it!=map_value.end(); it++){
      cout << "  " << it->first << "=" << it->second << endl;
    }
    Work work;
/*
    client.ping();
    COUT << "ping()" << endl;

    COUT << "1 + 1 = " << client.add(1, 1) << endl;

    work.op = Operation::DIVIDE;
    work.num1 = 1;
    work.num2 = 0;

    try {
      client.calculate(1, work);
    //  COUT << "Whoa? We can divide by zero!" << endl;
    } catch (InvalidOperation& io) {
      COUT << "InvalidOperation: " << io.why << endl;
      // or using generated operator<<: COUT << io << endl;
    }
*/
    time_t begin = PR_Now();
    int cnt = 100;
    for(int j=0;j<256*1024;j++){
      work.comment.push_back('a');
    }
    for(int i=0; i<cnt; i++){
      work.op = Operation::SUBTRACT;
      work.num1 = 15;
      work.num2 = 10;
      client.calculate(id, work);
    }
    time_t end = PR_Now();
    COUT << "calculate(sub) " << cnt << " times in " << end - begin << "ms" << endl;

    // Note that C++ uses return by reference for complex types to avoid
    // costly copy construction
 //   SharedStruct ss;
  //  client.getStruct(ss, 1);
   // COUT << "Received log: " << ss << endl;

    transport->close();
  } catch (TException& tx) {
    COUT << "ERROR: " << tx.what() << endl;
  }

}
