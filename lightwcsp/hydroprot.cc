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

//http://www.linuxhowtos.org/C_C++/socket.htm

#define PORT "2000"  // the port users will be connecting to

#define BACKLOG 10     // how many pending connections queue will hold

#define BUFF_SIZE 1024
#define URL_SIZE 512
#define MAX_URL_SIZE 4096

using namespace std;

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


int startup(u_short& port) {
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

	port = atoi(PORT);
	return sockfd;
}

void error_die(const char *sc) {
	perror(sc);
	exit(1);
}

int main(void) {
	u_short port = 0;
	int client_sock = 0;
	struct sockaddr_in client_name;
	socklen_t client_name_len = sizeof(client_name);

	int server_sock = -1;
	server_sock = startup(&port);
	printf("hydroprot running on port %d\n", port);

	client_sock = accept(server_sock, (struct sockaddr *) &client_name,
											 &client_name_len);
	cout << "accepted socket on main thread: " << client_sock << "\n";
	if (client_sock >= 0) {
		int client = client_sock;
		numchars = recv(client, buf, BUFF_SIZE - 1, 0);
		// send back dynamic content below...
		send(client_sock, fs->get_buf(fl), fs->get_filelen(fl), 0);
		close(client_sock);

	} else {
		error_die("accept");
	}

	close(server_sock);
	return (0);
}

