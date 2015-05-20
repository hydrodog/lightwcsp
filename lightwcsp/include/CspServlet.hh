#ifndef CSPSERVLET_HH_
#define CSPSERVLET_HH_

#include <iostream>
#include <string>
#include <unordered_map>
#include <dirent.h>            // 提供目录流操作函数  
#include "HttpRequest.hh"

class HttpRequest;
class HttpServlet {
private:
	static std::unordered_map<std::string, HttpServlet*> servlets;
public:
	static void init();
	HttpServlet() {}
	virtual void doGet(HttpRequest& req)=0;
	virtual ~HttpServlet() {}
};

class InMemoryFileServlet : public HttpServlet {
private:
  char* buf;
  size_t buflen;
 public:
  InMemoryFileServlet(struct dirent* file);
  void doGet(HttpRequest& req);
};

class FileServlet : public HttpServlet {
private:
	std::string filename;
public:
	FileServlet(struct dirent* dir);
	void doGet(HttpRequest& req);
};

class DirServlet : public HttpServlet {
 public:
	DirServlet(struct dirent* dir);
	void doGet(HttpRequest& req);
};

class CspCompilerServlet : public HttpServlet {
 public:
	CspCompilerServlet(struct dirent* dir);
	void doGet(HttpRequest& req);
};

class  AA: public HttpServlet {
public:
	AA(){std::cout<<"in AA"<<std::endl;};
	void doGet(HttpRequest& req);
};

class  BB: public HttpServlet {
public:
	BB(){std::cout<<"in BB"<<std::endl;};
	void doGet(HttpRequest& req);
};

class  test100: public HttpServlet {
public:
	test100(){};
	void doGet(HttpRequest& req);
};

class  test1k: public HttpServlet {
public:
	test1k(){};
	void doGet(HttpRequest& req);
};
class  test10k: public HttpServlet {
public:
	test10k(){};
	void doGet(HttpRequest& req);
};
class  test100k: public HttpServlet {
public:
	test100k(){};
	void doGet(HttpRequest& req);
};
class  test1m: public HttpServlet {
public:
	test1m(){};
	void doGet(HttpRequest& req);
};

class  CC: public HttpServlet {
public:
	CC(){std::cout<<"in CC"<<std::endl;};
	void doGet(HttpRequest& req);
};
#endif
