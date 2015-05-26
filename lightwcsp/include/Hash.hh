#ifndef HASHMAP_HH_
#define HASHMAP_HH_

#include <string>
#include <vector>
class HttpServlet;
class HashMap {
private:
  struct Node {
		std::string str;
    HttpServlet* servlet;
	Node* next;
		Node(std::string str= "",HttpServlet* ser=nullptr,Node* n=nullptr)
	:str(str),servlet(ser),next(n){}
  };
	std::vector<Node*> v_;
  size_t size_;
  size_t used_;
  
  size_t hash(const char* url) const;
  void grow();
public:
  HashMap(size_t size=50,size_t used=0);
  ~HashMap();
  /**
   * Example URLS:
   * pointer will be to the first character after / beyond machine
   * http://localhost:8080/dir/thisismyurl.html (stop at space)
   * http://localhost:8080/dir/thisismyurl.csp? 
   *  in the second case, stop at ? "dir/thisismyurl.csp"
   */
  void add(const char* url, HttpServlet* servlet);
  HttpServlet* get(const char* url) const;
};
#endif
