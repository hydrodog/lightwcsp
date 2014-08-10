#include "Logger.h"
#include "Config.h"
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
#include <sys/stat.h>
#include <signal.h>

#include <string>
#include <cstring>
//#include <regex>
#include <thread>
#include <server.h>
#include <RequestHandler.h>

#define SERVER_STRING "Server: lightwcsp/0.1.0\r\n"

enum {
	BACKLOG = 10
};
// how many pending connections queue will hold

enum {
	BUFF_SIZE = 1024
};
enum {
	URL_SIZE = 512
};
enum {
	MAX_URL_SIZE = 4096
};
// way too big? Just thinking about it holding the whole path too.
// Although, that didn't happen anyway.
int THREAD_POOL_SIZE;

using namespace std;

Config config; // create a config object
Logger logger;

// the handler cannot be inline because its pointer gets called
void sigchld_handler(int s) {
	while (waitpid(-1, NULL, WNOHANG) > 0)
		;
}

inline void *get_in_addr(struct sockaddr *sa) {
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*) sa)->sin_addr);
	}
	return &(((struct sockaddr_in6*) sa)->sin6_addr);
}

void accept_request(int client) {
	char buf[BUFF_SIZE];
	int numchars = 1;
	int cgi = 0; /* becomes true if server decides this is a CGI
	 * program */

	RequestHandler myReq(client);
	char url[MAX_URL_SIZE];
	strncpy(url, myReq.getUrl(), myReq.getUrlLength() + 1);
	int urlPos = 0;
	cout << "URL is: " << url << "\n";
	switch (myReq.getMethod()) {
	case GET:
		while (url[urlPos] != 0 && url[urlPos] != '?')
			urlPos++;
		url[urlPos] = 0; //We are temporarily discarding fields. TODO FIX
		break;
	case POST:
		cgi = 1;
		break;
	default:
		break;
	}
	char path[4200];
	if (url[0] == '/') {
		snprintf(path, 4200, "htdocs%s", url);
	}
	if (path[strlen(path) - 1] == '/') {
		strcat(path, "index.html");
	}
	while (recv(client, buf, BUFF_SIZE - 1, 0))
		;
	struct stat st; // = nullptr;
	if (stat(path, &st) == -1) {
		not_found(client);
	} else {
		if (S_ISDIR(st.st_mode)) {
			strcat(path, "/index.html");
		}
//    if ((st.st_mode & S_IXUSR) || (st.st_mode & S_IXGRP)
//	|| (st.st_mode & S_IXOTH)) {
//      cgi = 1;
//    }
		if (!cgi) {
			serve_file(client, path);
		}
		//        else
		//            execute_cgi(client, path, method, query_string);
	}
	close(client);
	logger.message(Logger::DETAILS, Logger::ENDED_THREAD);
}

void unimplemented(int client) {
	char buf[BUFF_SIZE];
	snprintf(buf, BUFF_SIZE,
			"HTTP/1.1 501 Method Not Implemented\r\n%sContent-Type:\
			text/html\r\n\r\n<HTML><HEAD><TITLE>Method Not Implemented\r\n</TITLE>\
             </HEAD>\r\n<BODY><P>HTTP request method not supported.\r\n</BODY></HTML>\r\n",
			SERVER_STRING);
	send(client, buf, strlen(buf), 0);
}

void not_found(int client) {
	char buf[BUFF_SIZE];
	snprintf(buf, BUFF_SIZE,
			"HTTP/1.1 404 NOT FOUND\r\n%sContent-Type: text/html\r\n\r\n<HTML><TITLE>Not Found</TITLE>\r\n\
             <BODY><P>The server could not fulfill\r\nyour request because the resource specified\r\n \
             is unavailable or nonexistent.\r\n</BODY></HTML>\r\n",
			SERVER_STRING);
	send(client, buf, strlen(buf), 0);
}

void sendStandardMessage(int client, int messageType) {
	send(client, config.getStandardMessage(messageType).c_str(),
			config.getStandardMessage(messageType).length(), 0);
}

void serve_file(int client, const char *filename) {
	FILE *resource = NULL;
	//    while ((numchars > 0) && strcmp("\n", buf))  /* read & discard headers */
	//    	numchars = recv(client, buf, BUFF_SIZE-1,0);

	resource = fopen(filename, "r");
	if (resource == NULL)
		not_found(client);
	else {
		headers(client, filename);
		cat(client, resource);
	}
	fclose(resource);
}

void headers(int client, const char filename[]) {
	char buf[BUFF_SIZE];
	//(void) filename; /* could use filename to determine file type */

	snprintf(buf, BUFF_SIZE,
			"HTTP/1.1 200 OK\r\n%sContent-Type: text/html\r\n\r\n",
			SERVER_STRING);
	logger.message(Logger::DETAILS, Logger::SERVER_OK);
	cout << buf << "\n";
	send(client, buf, strlen(buf), 0);
}

void cat(int client, FILE *resource) {
	char buf[BUFF_SIZE];
	int totalBytes;
	while ((totalBytes = fread(buf, 1, BUFF_SIZE, resource)) > 0) {
		send(client, buf, totalBytes, 0);
	}
	send(client, "\r\n", 2, 0);
}

int startup(u_short &port) {
	int sockfd;  // listen on sock_fd, new connection on new_fd
	struct addrinfo hints, *servinfo, *p;
	struct sigaction sa;
	int yes = 1;
	int rv;
	char portString[6];
	snprintf(portString, 5, "%d", port);

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;  // Make connection IP version agnostic
	hints.ai_socktype = SOCK_STREAM; // Will use Stream sockets and therefore, TCP
	hints.ai_flags = AI_PASSIVE; // use my IP

	if ((rv = getaddrinfo(NULL, portString, &hints, &servinfo)) != 0) {
		logger.fatal(Logger::GET_ADDR_INFO);
	}

	// loop through all the results and bind to the first we can
	for (p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol))
				== -1) {
			//TODO: We could log different error messages here, like port busy for example
			logger.message(Logger::ERROR, Logger::CANNOT_OPEN_SOCKET);
			continue;
		}

		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))
				== -1) {
			logger.fatal(Logger::SETSOCKOPT);
		}

		if (::bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			logger.message(Logger::ERROR, Logger::BIND);
			continue;
		}

		break;
	}

	if (p == NULL) {
		logger.fatal(Logger::FAILED_TO_BIND);
	}

	freeaddrinfo(servinfo); // all done with this structure

	if (listen(sockfd, BACKLOG) == -1) {
		logger.fatal(Logger::LISTEN_FAILED);
	}

	sa.sa_handler = sigchld_handler; // reap all dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		logger.fatal(Logger::SIG_ACTION);
	}
	return sockfd;
}

vector<thread> threadPool;

inline void spawnRequest(int clientSock) {
	if (threadPool.size() > 0) {
		thread t = threadPool.pop_back();
		t = thread(accept_request, clientSock);
		logger.message(Logger::INFO, Logger::STARTED_THREAD);
	} else {
		logger.fatal(Logger::EXHAUSTED_THREADS);
		//TODO: queue this up, wait until a thread is available
		// right now, the server will serve until there are
		// no threads left, then terminate.
	}
}

int main() {
	config.load(); // load the configurations
	logger.loadConfig();
	u_short port = config.getInt("port", 80);
	threadPool.reserve(config.getInt("threadpoolsize", 16));
	struct sockaddr_in client_name;
	socklen_t client_name_len = sizeof(client_name);
	int serverSock = -1;
	serverSock = startup(port);
	logger.message(Logger::DETAILS, Logger::RUNNING, port);

	int clientSock = 0;
	while (clientSock >= 0) {
		logger.message(Logger::INFO, Logger::ABOUT_TO_ACCEPT);
		clientSock = accept(serverSock, (struct sockaddr *) &client_name,
				&client_name_len);
		logger.message(Logger::INFO, Logger::ACCEPTED_SOCKET, clientSock);
		if (clientSock >= 0) {
			// original unthreaded request:
			//    accept_request(client_sock);
			spawnRequest(clientSock);
		}
	}

	close(serverSock);
	return 0;
}
