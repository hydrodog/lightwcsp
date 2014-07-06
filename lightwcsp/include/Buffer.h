#ifndef BUFFER_H__
#define BUFFER_H__

#include "DynList.h"
#include <string>

class Buffer {
private:
  char* buffer;
  char* p;
  int used;
  int size;
  void checkSpace(int n);
public:
  Buffer() {}
  Buffer(int sz) : size(sz), used(0) {
    buffer = new char[size];
    p = buffer;
  }

  ~Buffer() { delete [] buffer; }
  Buffer(const Buffer& orig); // copy not supported for now?
  Buffer& operator =(const Buffer& orig);

  void flush();
  void load(const char filename[]);
  void append_printf(const char fmt[], int estSize, ...);
  void append(const char buf[], int n);
  void append(const std::string& s);
  void append(int val);
  void append(float val);
  void append(double val);
  void append(int vals[], int n, const char sep[]);
  void append(float vals[], int n, const char sep[]);
  void append(double vals[], int n, const char sep[]);

  // copy this buffer to another one, adding in dynamic content
  void writeToBuffer(Buffer& buf, DynList& dynlist);
};

#endif
