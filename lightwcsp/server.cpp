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

#include <iostream>
#include <string>
#include <cstring>
#include <regex>
#include <thread>
#include <server.h>
#include <RequestHandler.h>

#define SERVER_STRING "Server: lightwcsp/0.1.0\r\n"

#define PORT "80"  // the port users will be connecting to

#define BACKLOG 10     // how many pending connections queue will hold

#define BUFF_SIZE 1024
#define URL_SIZE 512
#define MAX_URL_SIZE 4096

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

void inline *get_in_addr(struct sockaddr *sa) {
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*) sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*) sa)->sin6_addr);
}

void accept_request(int client) {
	char buf[BUFF_SIZE];
	int numchars;
	struct stat st;
	int cgi = 0; /* becomes true if server decides this is a CGI
	 * program */

	RequestHandler myReq(client);
	char url[MAX_URL_SIZE];
	strncpy(url, myReq.getUrl(), sizeof(myReq.getUrl()));
	int urlPos = 0;
	switch (myReq.getMethod()) {
	case GET:
		while (url[urlPos] != 0 && url[urlPos] != '?')
			;
		url[urlPos] = 0; //We are temporarily discarding fields. TODO FIX
		break;
	case POST:
		cgi = 1;
		break;
	default:
		break;
	}
	string path = string("htdocs") + string(url);
	if (path[path.size() - 1] == '/') {
		path += "index.html";
	}
	if (stat(path.c_str(), &st) == -1) {
		while ((numchars > 0)/*&& strcmp("\n", buf)*/) /* read & discard headers */
			numchars = recv(client, buf, BUFF_SIZE - 1, 0);
		not_found(client);
	} else {
		if ((st.st_mode & S_IFMT) == S_IFDIR)
			path += "index.html";
		if ((st.st_mode & S_IXUSR) || (st.st_mode & S_IXGRP)
				|| (st.st_mode & S_IXOTH))
			cgi = 1;
		if (!cgi)
			serve_file(client, path.c_str());
		//        else
		//            execute_cgi(client, path, method, query_string);
	}
	close(client);
}

void not_found(int client) {
	char buf[BUFF_SIZE];
	snprintf(buf, BUFF_SIZE,
			strcat(strcat("HTTP/1.0 404 NOT FOUND\r\n", SERVER_STRING),
					"Content-Type: text/html\r\n\r\n<HTML><TITLE>Not Found</TITLE>\r\n\
	    		<BODY><P>The server could not fulfill\r\nyour request because the resource specified\r\n\
				is unavailable or nonexistent.\r\n</BODY></HTML>\r\n"));
	send(client, buf, strlen(buf), 0);
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

void headers(int client, const char *filename) {
	char buf[BUFF_SIZE];
	(void) filename; /* could use filename to determine file type */

	strcpy(buf,
			strcat(strcat("HTTP/1.0 200 OK\r\n", SERVER_STRING),
					"Content-Type: text/html\r\n\r\n"));
	send(client, buf, strlen(buf), 0);
}

void cat(int client, FILE *resource) {
	char buf[BUFF_SIZE];
	while (fgets(buf, BUFF_SIZE, resource) != NULL) {
		if (feof(resource)) {
			send(client, buf, strlen(buf), 0);
			break;
		} else {
			send(client, buf, BUFF_SIZE - 1, 0);
		}

	}
}

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

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
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
			strcat(strcat("HTTP/1.0 501 Method Not Implemented\r\n",
			SERVER_STRING),
					"Content-Type: text/html\r\n\r\n<HTML><HEAD><TITLE>Method Not Implemented\r\n</TITLE>\
    		</HEAD>\r\n<BODY><P>HTTP request method not supported.\r\n</BODY></HTML>\r\n"));
	send(client, buf, strlen(buf), 0);
}

int main(void) {
	int server_sock = -1;
	u_short port = 0;
	int client_sock = -1;
	struct sockaddr_in client_name;
	int client_name_len = sizeof(client_name);

	server_sock = startup(&port);
	printf("httpd running on port %d\n", port);

	while (1) {
		client_sock = accept(server_sock, (struct sockaddr *) &client_name,
				&client_name_len);
		if (client_sock == -1)
			error_die("accept");
		/* accept_request(client_sock); */
		thread newThread(accept_request, client_sock);
	}

	close(server_sock);
	return (0);
}

