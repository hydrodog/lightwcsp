class test3_child : public HttpServlet
{
public:
	void doGet(HttpRequest& req)
	{
		Buffer& b = req.getOutput();
		b.append("<!DOCTYPE html>\n");
		b.append("<html lang=\"en\">\n");
		b.append("<head>\n");
		b.append("    <link rel=\"stylesheet\" href=\"style.css\" />\n");
		b.append("    <title>");
		b.append("Index");
		b.append("</title>\n");
		b.append("    ");
		b.append("<style type=\"text/css\">        .important { color: #336699; }    </style>");
		b.append("\n");
		b.append("</head>\n");
		b.append("\n");
		b.append("<body>\n");
		b.append("  <h1>");
		b.append("Index");
		b.append("</h1>\n");
		b.append("    <div id=\"content\">");
		b.append("<p class=\"important\">      Welcome to my awesome homepage.    </p>");
		b.append("</div>\n");
		b.append("    <div id=\"unsafe\" >");
		b.append("</div>\n");
		b.append("    <div id=\"footer\">\n");
		b.append("      ");
		b.append("\"Dov Kruger\"");
		b.append("\n");
		b.append("        ");
		b.append("&copy; Copyright 2015 by <a href=\"http://domain.invalid/\">you</a>.");
		b.append("\n");
		b.append("    </div>\n");
		b.append("</body>\n");
		b.append("</html>\n");
	}
};