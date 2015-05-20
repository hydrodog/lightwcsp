#include "CspServlet.hh"
#include <iostream>
#include <string>
using namespace std;

unordered_map<string, HttpServlet*> HttpServlet::servlets;

void HttpServlet::init(){
	HttpServlet::servlets["aa.csp"] = new AA();
	HttpServlet::servlets["bb.csp"] = new BB();
	HttpServlet::servlets["cc.csp"] = new CC();
	HttpServlet::servlets["100.csp"] = new test100();
	HttpServlet::servlets["1k.csp"] = new test1k();
	HttpServlet::servlets["10k.csp"] = new test10k();
	HttpServlet::servlets["100k.csp"] = new test100k();
	HttpServlet::servlets["1m.csp"] = new test1m();
}

//HttpRequest req(fileName); // initialize this object to contain:
	/*
		maintain session id
		setContentType("text/plain");
		stream to print to the client
		get parameters from forms
		get cookies
		invalidate session when it times out

	 */


void AA::doGet(HttpRequest& req) {
	// print out somethign to client
	//ClientStream& s = req.getClientStream();
	Buffer& b = req.getOutput();
	b.append("<html>\n<body>\n");
	for (int i = 0; i < 100; i++) {
		b.append(i);
		b.append("\n");
		}
	b.append("</body>\n</html>\n");
};

void test100::doGet(HttpRequest& req) {
	// print out somethign to client
	//ClientStream& s = req.getClientStream();
	Buffer& b = req.getOutput();
	b.append("<html>\n<body>\n");
	for (int i = 0; i < 100; i++) {
		b.append(1);
		//b.append("\n");
		}
	b.append("</body>\n</html>\n");
};
void test1k::doGet(HttpRequest& req) {
	// print out somethign to client
	//ClientStream& s = req.getClientStream();
	Buffer& b = req.getOutput();
	b.append("<html>\n<body>\n");
	for (int i = 0; i < 1000; i++) {
		b.append(1);
		//b.append("\n");
		}
	b.append("</body>\n</html>\n");
};
void test10k::doGet(HttpRequest& req) {
	// print out somethign to client
	//ClientStream& s = req.getClientStream();
	Buffer& b = req.getOutput();
	b.append("<html>\n<body>\n");
	for (int i = 0; i < 10000; i++) {
		b.append(1);
		//b.append("\n");
		}
	b.append("</body>\n</html>\n");
};

void test100k::doGet(HttpRequest& req) {
	// print out somethign to client
	//ClientStream& s = req.getClientStream();
	Buffer& b = req.getOutput();
	b.append("<html>\n<body>\n");
	for (int i = 0; i < 100000; i++) {
		b.append(1);
		//b.append("\n");
		}
	b.append("</body>\n</html>\n");
};

void test1m::doGet(HttpRequest& req) {
	// print out somethign to client
	//ClientStream& s = req.getClientStream();
	Buffer& b = req.getOutput();
	b.append("<html>\n<body>\n");
	for (int i = 0; i < 1000000; i++) {
		b.append(1);
		//b.append("\n");
		}
	b.append("</body>\n</html>\n");
};
void BB::doGet(HttpRequest& req) {
	// print out somethign to client
	//ClientStream& s = req.getClientStream();
	Buffer& b = req.getOutput();
	b.append("<html>\n<body>\n");
	for (int i = 0; i < 100; i++) {
		b.append(i*i);
		//b.append("\n");
		}
	b.append("</body>\n</html>\n");
};

void CC::doGet(HttpRequest& req) {
	// print out somethign to client
	//ClientStream& s = req.getClientStream();
	Buffer& b = req.getOutput();
	b.append("<html>\n<body>\n");
	for (int i = 0; i < 100; i++) {
		b.append(i+i);
		b.append("\n");
		}
	b.append("</body>\n</html>\n");
};
/*
A.csp
CSP Code eventually looks like this!

<%  for (int i = 0; i < 10; i++) { %>
test
<% } %>




generates:

class A : public HttpServlet {
public:
  virtual void doGet(HttpRequest& req) {
	  ClientStream& s = req.getClientStream();
    for (int i = 0; i < 10; i++) {
		s << "test";
   	}


  }

};

Compile using these g++ commands:
http://stackoverflow.com/questions/14884126/build-so-file-from-c-file-using-gcc-command-line


then load the .so file.  If already loaded, somehow unload old one and reload.
*/
