#include "CspServlet.hh"
#include <dirent.h>            // 提供目录流操作函数  
#include <server.hh>
#include <sys/socket.h>
#include <sys/stat.h>
#include <string>
#include <stdio.h>
using namespace std;

FileServlet::FileServlet(struct dirent* entry) {
  filename = entry->d_name;
}

void FileServlet::doGet(HttpRequest& req) {
  string filetype = filename.substr(filename.find_last_of(".")+1);
  const string& mimetype = mimeType[filetype];
  struct stat statbuf;
  lstat(filename.c_str(), &statbuf);  
  size_t filelen = statbuf.st_size;
  size_t buflen = filelen + HEADER_SIZE + mimetype.size();
	//     f->filedir = filedir + "/" + entry->d_name;
  FILE* inputf = fopen(filename.c_str(), "rb");
  char* buf;
  if (inputf == nullptr) cout << "cannot open " << filename;
  else {
    buf = new char[filelen];
    if (buf == nullptr){ //TODO: should be unnecessary
      cerr << "Memory error" << endl;
      exit(2);
    }
  }
	// preload the header information into the buffer
	char* buf2 = headers(buf, mimetype);
	// load file into the buffer
	int bytesRead = fread(buf2, filelen, 1, inputf);
	fclose(inputf);
	req.send(buf, buflen);
}
