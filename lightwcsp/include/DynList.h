#ifndef DYNLIST_H_
#define DYNLIST_H_

#include <vector>
class DynList {
private:
  struct Format {
    char type;
    int  offset : 24;
    void* ptr;
  };
  std::vector<Format> format;
public:
  DynList() {}
  void add(int offset, int v);
  void add(int offset, float v);
  void add(int offset, double v);
  void add(int offset, int v[], int n); // add n numbers
  template<typename T>
    void add(int offset, T v[], int T::*p, int n); // add n numbers in objects;
  template<typename T>
    void add(int offset, T* v[], int T::*p, int n); // add n numbers in objects;
  
  
};

#endif
