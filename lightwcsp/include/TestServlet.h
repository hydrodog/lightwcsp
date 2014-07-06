#ifndef TESTSERVLET_H__
#define TESTSERVLET_H__
class TestServlet : public Servlet {
 public:
  void doGet(HttpConn& conn);
  void doPost(HttpConn& conn);
};
#endif
