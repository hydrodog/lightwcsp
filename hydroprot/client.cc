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
// #include <string>
#include <cstring>
#include <iomanip>
#include <stdio.h>
#include <regex>

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

	// cout << "Enter the message: ";
	// cin >> buff;

	// if(write(sockfd,buff,strlen(buff)) < 0)
	// 	error_die("Error writing to socket");

	if(read(sockfd,buff,BUFFSIZE - 1) < 0)
		error_die("Error reading from socket");

	float v[9];
	sscanf(buff,"%f,%f,%f,%f,%f,%f,%f,%f,%f",v,v+1,v+2,v+3,v+4,v+5,v+6,v+7,v+8);

	cout << '$' << fixed << setprecision(2) << v[0] << endl;
	cout << '$' << fixed << setprecision(2) << v[1] << endl;
	cout << '$' << fixed << setprecision(2) << v[2] << endl;
	cout << '$' << fixed << setprecision(2) << v[3] << endl;
	cout << '$' << fixed << setprecision(2) << v[4] << endl;
	cout << '$' << fixed << setprecision(2) << v[5] << endl;
	cout << '$' << fixed << setprecision(2) << v[6] << endl;
	cout << '$' << fixed << setprecision(2) << v[7] << endl;
	cout << '$' << fixed << setprecision(2) << v[8] << endl;

	close(sockfd);


	return 0;
}
