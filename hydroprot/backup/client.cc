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
// #include <cstdio>
#include <fstream>

using namespace std;

// http://www.linuxhowtos.org/C_C++/socket.htm

#define PORT "2000"
#define BACKLOG 10
#define BUFFSIZE 1024

struct addrinfo *result;
int sockfd;

void signal_handler(int sig)
{
	cout << "Caught signal " << sig << endl;
	if(sockfd > 0)
		close(sockfd);
	exit(sig);
}

void error_die(const char *s)
{
	perror(s);
	exit(1);
}

int main(int argc,char *argv[])
{
	if(argc < 3)
	{
		cout << "IP address or option is missing" << endl;
		return 0;
	}

	signal(SIGINT, signal_handler);

	// int n = atoi(argv[1]);
	// int sockfd;
	struct addrinfo hints, /* result, */ *p;
	char buff[BUFFSIZE];

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;  // Make connection IP version internet
	hints.ai_socktype = SOCK_STREAM; // Will use Stream sockets / TCP
	hints.ai_flags = 0;
	hints.ai_protocol = 0;

	if(getaddrinfo(argv[1],PORT,&hints,&result))
		error_die("getaddrinfo");

	float v[9];

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

	// cout << "Enter the message: ";
	// cin >> buff;
	int ch = atoi(argv[2]);

	if(write(sockfd,&ch,sizeof(ch)) < 0)
		error_die("Error writing to socket");
	if(ch)
	{
		if(read(sockfd,buff,BUFFSIZE - 1) < 0)
			error_die("Error reading from socket");
		sscanf(buff,"%f,%f,%f,%f,%f,%f,%f,%f,%f",v,v+1,v+2,v+3,v+4,v+5,v+6,v+7,v+8);
	}
	else
	{
		ofstream out("static.dat");
		if(read(sockfd,buff,BUFFSIZE - 1) < 0)
			error_die("Error reading from socket");
		out << buff;
		out.close();
	}


	close(sockfd);

	freeaddrinfo(result);

	/*
	// File reconstruction
	ifstream fixed("fixed.dat");
	ifstream vec("vec.dat");

	if(fixed.is_open() && vec.is_open())
	{
		cout << "Unable to open files" << endl;
		return 0;
	}

	ofstream file("rec.html");

	string vline;
	string fline;
	int cline = 0;
	int i = 0;

	while(getline(vec,vline))
	{
		if(vline[0]=='-')
		{
			if(i)
				file << (fline.c_str()+i) << endl;
			while(getline(fixed,fline))
				file << fline << endl;
			break;
		}
		else
		{
			int l,c,n;
			sscanf(vline.c_str(),"%d,%d,%d",&l,&c,&n);
			if((cline < l)&&i)
			{
				file << (fline.c_str()+i) << endl;
				cline++;
				i = 0;
			}
			while(cline < l)
			{
				getline(fixed,fline);
				file << fline << endl;
				cline++;
				i = 0;
			}
			if(cline==l)
			{
				if(!i)
					getline(fixed,fline);
				for(; i<c; i++)
					file << fline[i];
				file << v[n];
			}
		}
	}
	file.close();
	fixed.close();
	vec.close();*/

	return 0;
}