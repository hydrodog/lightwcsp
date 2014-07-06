#include "TestServlet.h"

void TestServlet::doGet(HttpConn& conn) {
  Buffer& buf = conn.getBuffer();
  buf.append
    (
     "HTTP 1.1 200 OK\n"
     "Content-type: text/html; charset=UTF-8\n"
     "\n\n"
     "<html>"
     "<head>"
     "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">"
     "<title>Dov Test</title>"
     "</head>"
     "<body>"
     "<table>"
     );
  for (int i = 1; i <= 1000; i++) {
    buf.append("<tr><td>");
    buf.append(i);
    buf.append("</td></tr>");
  }

  buf.append
    (
     "</table>"
     "</body>"
     "</html>"
     );
}
