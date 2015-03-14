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
#include <CspServlet.h>
#include <iostream>
#include <string>
#include <cstring>
#include <regex>
#include <thread>
#include <server.h>
#include <RequestHandler.h>
#include <FileSys.h>

#define SERVER_STRING "Server: lightwcsp/0.1.0\r\n"

#define PORT "80"  // the port users will be connecting to

#define BACKLOG 10     // how many pending connections queue will hold

#define BUFF_SIZE 1024
#define URL_SIZE 512
#define MAX_URL_SIZE 4096
#define THREAD_POOL_SIZE 100

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

void accept_request(int client,FileSys* fs) {
	char buf[BUFF_SIZE];
	int numchars;
	struct stat st;
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
	cout << "Made it to file opening. Opening in path: " << path << "\n";
	if (stat(path, &st) == -1) { //test whether is a path OR a dir
		cout<<"test point 1 : be in if(stat(path,&st)==-1)"<<endl;
		
		while ((numchars > 0)/*&& strcmp("\n", buf)*/) /* read & discard headers */
			numchars = recv(client, buf, BUFF_SIZE - 1, 0);
		not_found(client);
	} else {
		if ((st.st_mode & S_IFMT) == S_IFDIR) { //if it is a dir, return index
			cout << "First if\n";
			strcat(path, "index.html");
		}
		if ((st.st_mode & S_IXUSR) || (st.st_mode & S_IXGRP) // access for file
				|| (st.st_mode & S_IXOTH)) {
			//cgi = 1;
			if (!(st.st_mode & S_IXUSR)) cout <<"reject to transfer file: not a qualified user"<<endl;
			if (!(st.st_mode & S_IXGRP)) cout <<"reject to transfer file: not a qualified group"<<endl;
			if (!(st.st_mode & S_IXOTH)) cout <<"reject to transfer file: do not have access to read/write"<<endl;
			cgi= 0;
		}
		if (!cgi) {
			cout << "Serving file: " << path << "\n";
			//serve_file(client, path);
			FL* fl= fs->access((string)path);
			if (!fl) {
				cout<<"not find key "<<path<<endl;
			}
			else{
				if (fl->filetype == "csp"){
					HttpServlet temp;
					string res = temp.dispatcher(fl->filename);
					headers(client,path);
					send(client, res.c_str(), res.length(), 0);
					send(client, "\r\n", 2, 0);
				}
				else{
				headers(client,path);
				send(client, fs->get_buf(fl), fs->get_filelen(fl), 0);
				send(client, "\r\n", 2, 0);
				}
			}
		}
		//        else
		//            execute_cgi(client, path, method, query_string);
	}
	close(client);
	cout << "End thread" << endl;
}

void not_found(int client) {
	char buf[BUFF_SIZE];
	snprintf(buf, BUFF_SIZE,
			"HTTP/1.1 404 NOT FOUND\r\n%sContent-Type: text/html\r\n\r\n<HTML><TITLE>Not Found</TITLE>\r\n\
             <BODY><P>The server could not fulfill\r\nyour request because the resource specified\r\n\
             is unavailable or nonexistent.\r\n</BODY></HTML>\r\n",
			SERVER_STRING);
	send(client, buf, strlen(buf), 0);
}



void serve_file(int client, const char *filename) {
	 FILE *resource = NULL;
	//    while ((numchars > 0) && strcmp("\n", buf))  //read & discard headers 
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
	//(void) filename; /* could use filename to determine file type */

	snprintf(buf, BUFF_SIZE,
			"HTTP/1.1 200 OK\r\n%sContent-Type: text/html\r\n\r\n",
			SERVER_STRING);
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

int main(void) {
	int server_sock = -1;
	u_short port = 0;
	int client_sock = 0;
	struct sockaddr_in client_name;
	socklen_t client_name_len = sizeof(client_name);
	thread threadPool[THREAD_POOL_SIZE];

	server_sock = startup(&port);
	printf("httpd running on port %d\n", port);
	int nextThread = 0;

	//prepare for the file(Traversal all the file under htdocs)
	FileSys FS;
	while (client_sock >= 0) {
		cout << "made it to the beginning of cycle" << endl;
		client_sock = accept(server_sock, (struct sockaddr *) &client_name,
				&client_name_len);
		cout << "accepted socket on main thread: " << client_sock << "\n";
		if (client_sock >= 0) {
			/* accept_request(client_sock); */
			cout << "Starting new thread\n";
			while(threadPool[nextThread].joinable()){
				nextThread++;
			}
			threadPool[nextThread++] = thread(accept_request, client_sock,&FS);
			cout << "New thread started\n";
			if (nextThread == 100)
				nextThread = 0;
		} else {
			error_die("accept");
		}
	}

	close(server_sock);
	return (0);
}

