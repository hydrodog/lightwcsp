unordered_map<string name, HttpServlet*> servlets;

servlets["Something"] = new Something();
servlets["b"] = new B();

void dispatcher(string name) { // Something.csp
	HttpRequest req; // initialize this object to contain:
	
	/*
maintain session id
		setContentType("text/plain");
		stream to print to the client
		get parameters from forms
		get cookies
		invalidate session when it times out

	 */
	servlets[name]->doGet(req);

}

class HttpServlet {
public:
  virtual void doGet(HttpRequest& req) = 0;
	virtual ~HttpServlet() {}
};

class Something : public HttpServlet {
public:
  virtual void doGet(HttpRequest& req) {
		// print out somethign to client
	  ClientStream& s = req.getClientStream();
		s << "<html><body>test</body></html>";
   	}
  }

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
