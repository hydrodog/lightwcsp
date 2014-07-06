#include "Servlet.h"
#include "ServletHashTable.h" // not implemented

// create a mapping that looks up char* name of servlet,
// returns corresponding servlet.
// if null, get out.
ServletHashMap servletDispatch;
void createDispatch() {
  // this is a hardcoded example
  servletDispatch.add("x", new TestServlet()); // eventually compile .csp files
}
void dispatch(const char* servlet, int length) {
  Servlet* p = servletDispatch.lookup(servlet, length);
  if (p == NULL) {
    // servlet not in table
    // emit some error?
    return; // get out, no permission to run
  }
  //TODO: get user permissions from the client.
  int userperm = 0;
  p->dispatch(userperm); // call the servlet, checking if there is permission
}
