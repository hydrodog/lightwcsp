#include <iostream>
#include <fstream>
#include <unistd.h>
#include "Bench.h"
using namespace std;

long long f(int n) {
  long long sum = 0;
  for (int i = 0; i < n; i++)
    sum += i;
  return sum;
}

void g(int n) {
  for (int i = 0; i < n; i++) {
    ofstream f("test.dat");
    f << "test" << i << "\n";
  }
}

    
int main() {
  Bench b1;

  b1.start();
  f(100000000);
  b1.stop();
  cout << b1;

  b1.start();
  f(100000000);
  b1.stop();
  cout << b1;

  Bench b2;
  b2.start();
  g(100000);
  b2.stop();
  cout << b2;

  Bench b3;
  b3.start();
  sleep(20);
  b3.stop();
  cout << b3;
}
