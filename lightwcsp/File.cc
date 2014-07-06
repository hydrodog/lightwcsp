#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "File.h"
char* File::readFully(const char filename[]) {
  int fh = open(filename, O_RDONLY);
  if (fh < 0)
    return NULL;
  struct stat s;
  fstat(fh, &s);
  char* p = new char[s.st_size];
  int bytesRead = read(fh, p, s.st_size);
  if (bytesRead < s.st_size) {
    delete [] p;
    return NULL;
  }
  return p;
}
