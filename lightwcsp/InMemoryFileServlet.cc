#include "CspServlet.hh"
#include <dirent.h>            // 提供目录流操作函数  
#include <string>
#include <server.hh>
#include <sys/stat.h>
#include <sys/socket.h>
#include <FileSys.hh>
#include <iostream>
using namespace std;

InMemoryFileServlet::InMemoryFileServlet(struct dirent* entry) {
  string filename = entry->d_name;
  string filetype = filename.substr(filename.find_last_of(".")+1);
  const string& mimetype = mimeType[filetype];
  struct stat statbuf;
  lstat(entry->d_name, &statbuf);
  size_t filelen = statbuf.st_size + HEADER_SIZE + mimetype.size();
	//     f->filedir = filedir + "/" + entry->d_name;
  FILE* inputf = fopen(filename.c_str(), "rb");
  if (inputf == nullptr) cout << "cannot open " << entry->d_name;
  else {
    buf = new char[filelen];
    if (buf == nullptr){ //TODO: should be unnecessary
      cerr << "Memory error" << endl;
      throw("Memory error in InMemoryFileServlet");
    }
  }
  // preload the header information into the buffer
  char* buf2 = headers(buf, mimetype);
  // load file into the buffer
  int bytesRead = fread(buf2, filelen, 1, inputf);
  fclose(inputf);

}

void InMemoryFileServlet::doGet(HttpRequest& req) {
  req.send(buf, buflen);

}
