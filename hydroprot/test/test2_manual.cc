class Test2 : public HttpServlet {
public:
	void doGet(HttpRequest& req) {
		Buffer& b = req.getOutput();
		b.append("<html>"
"<head>"
"<title>test</title>"
"</head>"
"<body>"
						 "<table>");
		int n = 27;
		b.append("  <tr><td> ");
		b.append(n);
		b.append("</td></tr>");
		while (n > 1) {
			if (n % 2 == 0)
				n /= 2;
			else
				n = n * 3 + 1;
			b.append("  <tr><td> ");
			b.append(n);
			b.append("</td></tr>");
		}
	}
};
