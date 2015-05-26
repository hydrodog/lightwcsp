/*
* server.h
*
*  Created on: Feb 14, 2015
*      Author: Daniel
*/
// time mea load -> server -> shut down  
// one branch decide 1 access 2 what file type 3 JSP 
// encryption in file. 

#include <fcntl.h>             // 提供open()函数  
#include <unistd.h>  
#include <stdio.h>  
#include <string.h>  
#include <sys/stat.h>        // 提供属性操作函数  
#include <sys/types.h>         // 提供mode_t 类型  
#include <stdlib.h> 
#include <iostream>
#include <unordered_map>
#include <FileSys.hh>
#include <server.hh>
#include <CspServlet.hh>
using namespace std;

unordered_map<string,string> mimeType;
void initMimeTypes() {
  mimeType["html"] = "text/html";
  mimeType["txt"] = "text/plain";
  mimeType["xls"] = "application/vnd.ms-excel";
  mimeType["xlsx"] = "application/vnd.ms-excel";
  mimeType["zip"] = "application/x-gzip";
}

/**
 * Recursively descend through file system loading each file
 * FileSys caches each file with its header. Short files can be sent out
 * with a single send call.
 */
void FileSys::dfs(string filedir){
  DIR *dp;
  struct stat statbuf;
  if((dp = opendir(filedir.c_str())) == nullptr) {
    cerr << "cannot open directory: " << filedir.c_str() << '\n';
    return;
  }
  chdir(filedir.c_str());
  struct dirent *entry;
  while((entry = readdir(dp)) != nullptr){
    lstat(entry->d_name,&statbuf);
	cout << entry->d_name << endl;
    if (S_IFDIR & statbuf.st_mode){
      if (strcmp(entry->d_name, ".") != 0 && (strcmp(entry->d_name, "..") != 0)){
	     cerr << "result=" << (chdir("htdocs") == 0) << endl;	
	     cerr << "result=" << (chdir("..") == 0) << endl;	
 	     dfs(filedir + "/" + entry->d_name);
      }
    } else {
			// IF .txt file then new InMemoryFileServlet(entry)
			// if .html file new InMemoryFileServlet(entry);
			// if .csp file new CspCompileServlet(entry);
			// if aa.csp ->  new AA();
			// if bb.csp -> new BB();
			filedir += "/";
			filedir += entry->d_name;

			HttpServlet* s = new InMemoryFileServlet(entry);
			
			filemap.insert({ filedir, s });
		}
  } 
  if (chdir("..") != 0) {
    cerr << "chdir .. failed\n";
  }
  closedir(dp);
}

FileSys::~FileSys(){
  for (unordered_map<string, HttpServlet*>::iterator i = filemap.begin();
       i != filemap.end(); ++i) {
    delete [] i->second;
  }
}

void FileSys::print() {}

#if 0
  for (auto i = filemap.begin(); i != filemap.end(); i++){
    cout << "hashkey:  "<< i->first << '\n';
    cout << "filedir:  "<< i->second->filedir  << '\n';
    cout << "filename: "<< i->second->filename << '\n';
    //	cout << i->second->buf << endl;
  }
}
#endif
