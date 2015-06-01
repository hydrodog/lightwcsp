class Test1 : public HttpServlet {
public:
	void doGet(HttpRequest& req) {
		Buffer& b = req.getOutput();
		b.append("<html>\n"
						 "<head>\n<title>test</title>\n"
						 "</head>\n");
		b.append("<body bgcolor=\"blue\">\n");
		b.append("&lt;%\n");
		for(int i = 1; i <= 100; i++) {
			b.append("<p>Test ");
			b.append(i);
			b.append("</p>\n");
			b.append("</body>\n");
			b.append("</html>\n");
		}
	}
};
