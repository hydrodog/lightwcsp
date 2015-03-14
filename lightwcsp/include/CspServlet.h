#ifndef CSPSERVLET_H_
#define CSPSERVLET_H_

#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;

class HttpServlet {
private:
	static unordered_map<string, HttpServlet*> servlets;
public:
	HttpServlet();
	string dispatcher(string filename);
	virtual string doGet();
	virtual ~HttpServlet() {}
};


class  AA: public HttpServlet {
public:
	AA(){cout<<"in AA"<<endl;};
	string doGet();
};

class  BB: public HttpServlet {
public:
	BB(){cout<<"in AA"<<endl;};
	string doGet();
};

class  CC: public HttpServlet {
public:
	CC(){cout<<"in AA"<<endl;};
	string doGet();
};
#endif