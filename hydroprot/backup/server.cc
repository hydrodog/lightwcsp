#include <iostream>
#include <cstdlib>
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
#include <cstring>
#include <string>
#include <csignal>
#include <fcntl.h>

using namespace std;

#define PORT "2000"
#define BACKLOG 10
#define BUFFSIZE 1024

int server_sock = -1;

// http://www.linuxhowtos.org/C_C++/socket.htm

// Using ipv4 or ipv6
// void inline *get_in_addr(struct sockaddr *sa)
// {
// 	if (sa->sa_family == AF_INET)
// 		return &(((struct sockaddr_in*) sa)->sin_addr);
// 	else
// 		return &(((struct sockaddr_in6*) sa)->sin6_addr);
// }

void signal_handler(int sig)
{
	cout << "Caught signal " << sig << endl;
	if(server_sock > 0)
		close(server_sock);
	exit(sig);
}

void error_die(const char *s)
{
	perror(s);
	exit(1);
}

int startup(uint16_t& port)
{
	int sockfd;
	struct addrinfo hints, *servinfo, *p;
	int yes = 1;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;  // Make connection IP version agnostic
	hints.ai_socktype = SOCK_STREAM; // Will use Stream sockets / TCP
	hints.ai_flags = AI_PASSIVE; // use my IP

	if(getaddrinfo(nullptr,PORT,&hints,&servinfo))
		error_die("getaddrinfo");

	// loop on linked list to bind
	for(p = servinfo; p; p = p->ai_next)
	{
		if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
		{
			perror("server: socket");
			continue;
		}

		if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)))
			perror("setsockopt");

		// tries to bind
		if (::bind(sockfd, p->ai_addr, p->ai_addrlen) < 0)
		{
			close(sockfd);
			perror("server: bind");
			continue;
		}

		break;
	}

	if(!p)
		error_die("Server failed to bind");

	// frees linked list's allocated memory
	freeaddrinfo(servinfo);

	if (listen(sockfd, BACKLOG) < 0)
		error_die("listen");

	port = atoi(PORT);
	return sockfd;
}

int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		cout << "File is missing" << endl;
		return 0;
	}

	struct stat fstats;
	int fd = open(argv[1],O_RDONLY);

	if(!fd)
	{
		cout << "Error opening the file" << endl;
		return 0;
	}
	if(fstat(fd,&fstats))
	{
		cout << "Error getting file stats" << endl;
		return 0;
	}

	signal(SIGINT, signal_handler);

	uint16_t port = 0;
	int client_sock = 0;
	// int server_sock = -1;;
	struct sockaddr_in client_name;
	socklen_t client_name_len = sizeof(client_name);

	server_sock = startup(port);
	cout << "Hydroprot running on port " <<  port << endl;
	cout << "Socket: " << server_sock << endl;

	char buff[BUFFSIZE];
	int opt;

	for(;;)
	{
		client_sock = accept(server_sock, (struct sockaddr *) &client_name, &client_name_len);
		// cout << "Accepted socket: " << client_sock << endl;

		if (client_sock >= 0)
		{
			// memset(buff,0,BUFFSIZE);
			
			// Read message (same as read())
			// numchars is the message lentgh, or -1 if there's an error
			int numchars = read(client_sock, &opt, sizeof(int));
			if(numchars < 0)
				perror("read");

			if(!opt)
			{
				read(fd,buff,BUFFSIZE);
				write(client_sock,buff,BUFFSIZE);
			}
			else
			{
				write(client_sock, "4988.36,00.00,4988.36,00.00,5000.00,11.64,5000.00,00.00,4988.36", BUFFSIZE - 1);
			}
			
			// Send message back (same as write())
			// write(client_sock, "4988.36,00.00,4988.36,00.00,5000.00,11.64,5000.00,00.00,4988.36", BUFFSIZE - 1);
			close(client_sock);
		}
		else
			perror("accept failed\n");
	}

	close(server_sock);
	return 0;
}