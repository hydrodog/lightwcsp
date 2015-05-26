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
#include <string>
#include <cstring>

using namespace std;

// http://www.linuxhowtos.org/C_C++/socket.htm

#define PORT "2000"
#define BACKLOG 10
#define BUFFSIZE 1024

void error_die(const char *s)
{
	perror(s);
	exit(1);
}

int main(int argc,char *argv[])
{
	if(argc < 2)
	{
		cout << "IP address is missing" << endl;
		return 0;
	}
	int sockfd;
	struct addrinfo hints, *result, *p;
	char buff[BUFFSIZE];

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;  // Make connection IP version internet
	hints.ai_socktype = SOCK_STREAM; // Will use Stream sockets / TCP
	hints.ai_flags = 0;
	hints.ai_protocol = 0;

	if(getaddrinfo(argv[1],PORT,&hints,&result))
		error_die("getaddrinfo");

	for(p = result; p; p = p->ai_next)
	{
		if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
		{
			perror("client: socket");
			continue;
		}

		if(!connect(sockfd, p->ai_addr, p->ai_addrlen))
			break;	// Success

		close(sockfd);
	}

	if(!p)
		error_die("Could not connect");

	freeaddrinfo(result);

	cout << "Enter the message: ";
	cin >> buff;

	if(write(sockfd,buff,strlen(buff)) < 0)
		error_die("Error writing to socket");

	if(read(sockfd,buff,BUFFSIZE - 1) < 0)
		error_die("Error reading from socket");

	cout << buff << endl;

	close(sockfd);


	return 0;
}