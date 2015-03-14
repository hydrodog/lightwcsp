#include "CspServlet.h"
#include <iostream>
#include <string>
using namespace std;


// string HttpRequest::get_name(){
	// return name;
// }
HttpServlet ::HttpServlet(){
		HttpServlet::servlets["aa.csp"] = new AA();
		HttpServlet::servlets["bb.csp"] = new BB();
		HttpServlet::servlets["cc.csp"] = new CC();
}

string HttpServlet::dispatcher(string fileName) { // Something.csp
	//HttpRequest req(fileName); // initialize this object to contain:
	/*
		maintain session id
		setContentType("text/plain");
		stream to print to the client
		get parameters from forms
		get cookies
		invalidate session when it times out

	 */
	unordered_map<string,HttpServlet*>::const_iterator got = HttpServlet::servlets.find(fileName);
	if (got == HttpServlet::servlets.end())
		return nullptr;
	else
		return HttpServlet::servlets[fileName]->doGet();
}

string HttpServlet::doGet(){
	cout<<"In HttpServlet.doGet function"<<endl;
	return "";
}


string AA::doGet() {
	// print out somethign to client
	//ClientStream& s = req.getClientStream();
	string s; 
	s += "<html>\n<body>\n";
	for (int i = 0; i < 100; i++) {
		s += i;
		s += "\n";
		}
	s += "</body>\n</html>\n";
	return s;
};


string BB::doGet() {
	// print out somethign to client
	//ClientStream& s = req.getClientStream();
	string s; 
	s += "<html>\n<body>\n";
	for (int i = 0; i < 100; i++) {
		s += i*i;
		s += "\n";
		}
	s += "</body>\n</html>\n";
	return s;
};

string CC::doGet() {
	// print out somethign to client
	//ClientStream& s = req.getClientStream();
	string s; 
	s += "<html>\n<body>\n";
	for (int i = 0; i < 100; i++) {
		s += i+i;
		s += "\n";
		}
	s += "</body>\n</html>\n";
	return s;
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
