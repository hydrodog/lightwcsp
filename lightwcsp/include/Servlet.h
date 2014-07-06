#ifndef SERVLET_H__
#define SERVLET_H__
/*
 * Abstract parent of all servlets.  Contains the permissions bits to determine
 * whether or not to run a servlet, as well as the abstract method spec to call
 * the child servlet.
 * @author Dov Kruger
 */
class Servlet {
private:
  int permissions;
public:
  Servlet(int perm) : permissions(perm) {}
  void dispatch(unsigned int userPerm) {
    if ((userPerm & permissions) != 0) { // at least one bit in common, run!
      // notice how the permissions bits happen very early.  If we don't have
      // permission, don't even bother to generate the connection context
      // probably do have to respond with some context however, like a 
      // 401 message "Unauthorized"
      // see http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html
      HttpConn conn; // TODO: Create the connection from the server context
      // now call the doGet method to run the servlet!
      doGet(conn);
    }
  }
  void doGet(HttpConn& conn) = 0;
  void doPost(HttpConn& conn) = 0;
};

#endif

