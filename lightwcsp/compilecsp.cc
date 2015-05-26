#include <iostream>
#include <regex>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
using namespace std;

string readFile(const char* filename) {
  int csp = open(filename, O_RDONLY);
  stat buf;
  fstat(csp, &buf);
  const size_t size = buf.st_size;
  char* p = new char[size];
  if (read(csp, p, size) < size)
    throw "Unable to read entire file\n";
  f.getline(buffer, BUFSIZE);
  return string(p, size);
}

int main(int argc, char* argv[]) {
  enum { BUFSIZE = 16384 };
  regex page("<%@page [^%]*%>");
  regex tag("<%[^%]*%>");
  for (int i = 1; i < argc; i++) {
    string csp = readFile(argv[i]);
    regex_search(csp, match, tag);

    delete [] buf;
  }
}
