#ifndef FILESYS_H_
#define FILESYS_H_

#include <iostream>
#include <unordered_map>
#include <dirent.h>            // 提供目录流操作函数  
#include <string>
#include <unistd.h>

class HttpServlet;
extern std::unordered_map<std::string,std::string> mimeType;

class FileSys {
private:
  std::unordered_map<std::string, HttpServlet*>filemap;
  void dfs(std::string filedir);
public:
  FileSys(const std::string& filedir) {
    dfs(filedir);
  }
  void update();
  ~FileSys();
  void encryption();
  void decryption();
  HttpServlet* access(const std::string& filename) {
    return filemap[filename];
  }
  void print();
};

#endif /* FILESYS_H_ */
