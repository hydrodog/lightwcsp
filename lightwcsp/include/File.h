#ifndef FILE_H__
#define FILE_H__

class Buffer;
class File {
 public:
  // open a file, and read it fully into a Buffer
  // appending to whatever is already there
  static void readFully(const char filename[], Buffer& buf);

  // write a buffer out to a file
  void write(const char filename[], const Buffer& buf);
};

#endif
