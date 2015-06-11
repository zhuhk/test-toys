#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "misc.h"

using namespace std;
void func(int cnt){
  float a = 1.1 , b=1.2;

  for(int i=0;i<cnt;i++){
    a *= b;
  }
}
void diff(int loopCnt, int cnt){
  time_t begin = PR_Now(), end;

  cout << " + loopCnt=" << loopCnt << " cnt=" << cnt <<endl;
  for(int i=0;i<loopCnt;i++){
    func(cnt);
  }

  end = PR_Now();
  cout << "   - serial:" << end - begin << "ms" <<endl;
  
  begin = end;
  #pragma omp parallel for
  for(int i=0;i<loopCnt;i++){
    func(cnt);
  }
  end = PR_Now();
  cout << "   - openmp:" << end - begin << "ms" <<endl;

}
int main(int argc, char**argv){
  diff(1,10000);
  diff(1,1000000);
  diff(10,10000);
  diff(10,1000000);
  diff(1000000,10);
  diff(10000,10000);

  return 0;
}
