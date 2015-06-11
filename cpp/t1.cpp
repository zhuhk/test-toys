#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "misc.h"

using namespace std;
void func(){
  float a = 1.1 , b=1.2;

  for(int i=0;i<100000;i++){
    a *= b;
  }
}
int main(int argc, char**argv){
  time_t begin = PR_Now(), end;

  for(int i=0;i<10;i++){
    func();
  }

  end = PR_Now();
  cout << "normal:" << end - begin << "ms" <<endl;
  
  begin = end;
  #pragma omp parallel for
  for(int i=0;i<10;i++){
    func();
  }
  end = PR_Now();
  cout << "openmp:" << end - begin << "ms" <<endl;

  return 0;
}
