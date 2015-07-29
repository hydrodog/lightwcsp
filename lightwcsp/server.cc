#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
//#include <sys/stat.h>
#include <signal.h>
#include <CspServlet.hh>
#include <iostream>
#include <string>
#include <cstring>
//#include <regex>
#include <thread>
#include <server.hh>
#include <FileSys.hh>
#include "Signal.hh"
#include <HttpsServer.hh>
#define SERVER_STRING "Server: lwc/1\r\n"
#define PORT "4040"  // the port users will be connecting to
#define BACKLOG 10   // how many pending connections queue will hold
#define BUFF_SIZE 1024
#define URL_SIZE 512
#define MAX_URL_SIZE 4096
#define THREAD_POOL_SIZE 10

using namespace std;

#if 0
// not implemented yet, get busy people!
Logger logger1;
Config config;// create a config object
#endif

void inline sigchld_handler(int s) {
	while (waitpid(-1, NULL, WNOHANG) > 0)
		;
}


#if 0
inline void *get_in_addr(struct sockaddr *sa) {
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*) sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*) sa)->sin6_addr);
}
#endif

void accept_request(int client) {
  HttpRequest req(client);
  req.run();
  close(client);
  //	cout << "End thread" << endl;
}

char NOT_FOUND[] =
 "HTTP/1.1 404 NOT FOUND\r\n"
	SERVER_STRING
	"Content-Type: text/html\r\n\r\n"
  "<HTML><TITLE>Not Found</TITLE>\r\n"
  "<BODY><P>The server could not fulfill\r\n"
  "your request because the resource specified\r\n"
	"is unavailable or nonexistent.\r\n</BODY></HTML>\r\n";


const char HEADER[] =
	"HTTP/1.1 200 OK\r\n"
	SERVER_STRING
	"Content-Type: "
	;

const unsigned int HEADER_SIZE = sizeof(HEADER)-1;

int startup(u_short *port) {
	int sockfd;  // listen on sock_fd, new connection on new_fd
	struct addrinfo hints, *servinfo, *p;
	struct sigaction sa;
	int yes = 1;
	int rv;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;  // Make connection IP version agnostic
	hints.ai_socktype = SOCK_STREAM; // Will use Stream sockets and therefore, TCP
	hints.ai_flags = AI_PASSIVE; // use my IP

	if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
		error_die("getaddrinfo");
	}

	// loop through all the results and bind to the first we can
	for (p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol))
				== -1) {
			perror("server: socket");
			continue;
		}
		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))
				== -1) {
			perror("setsockopt");
			exit(1);
		}
		if (::bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("server: bind");
			continue;
		}

		break;
	}

	if (p == NULL) {
		perror("Server failed to bind\n");
		exit(1);
	}

	freeaddrinfo(servinfo); // all done with this structure

	if (listen(sockfd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}

	sa.sa_handler = sigchld_handler; // reap all dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}

	*port = atoi(PORT);
	return sockfd;
}

void error_die(const char *sc) {
	perror(sc);
	exit(1);
}

void unimplemented(int client) {
	char buf[BUFF_SIZE];
	snprintf(buf, BUFF_SIZE,
			"HTTP/1.1 501 Method Not Implemented\r\n%sContent-Type: text/html\r\n\r\n<HTML><HEAD><TITLE>Method Not Implemented\r\n</TITLE>\
             </HEAD>\r\n<BODY><P>HTTP request method not supported.\r\n</BODY></HTML>\r\n",
			SERVER_STRING);
	send(client, buf, strlen(buf), 0);
}

void https(){
	HttpsServer h;
}
int main(int argc, char* argv[]) {
	
  int serverSock = -1;
  u_short port = 0;
  int clientSock = 0;
  struct sockaddr_in clientName;
  socklen_t clientNameLen = sizeof(clientName);
  thread threadPool[THREAD_POOL_SIZE];
  thread httpsThread(https);
  serverSock = startup(&port);
  cerr << "lwcsp running on port " << port << '\n';
  int nextThread = 0;
  while (clientSock >= 0) {
    clientSock = accept(serverSock, (struct sockaddr *) &clientName,
			 &clientNameLen);
    if (clientSock >= 0) {
      while(threadPool[nextThread].joinable()){
				nextThread++;
      }
      threadPool[nextThread++] = thread(accept_request, clientSock);
      if (nextThread >= THREAD_POOL_SIZE)
	nextThread = 0;
    } else {
      error_die("accept");
    }
  }
  close(serverSock);
  return 0;
}


char* headers(char* buf, const string& mimeType) {
	memcpy(buf, HEADER, HEADER_SIZE);
	buf += HEADER_SIZE;
	memcpy(buf, mimeType.c_str(), mimeType.length());
	buf += mimeType.length();
	memcpy(buf, "\r\n\r\n", 4);
  return buf + 4;
}
	
