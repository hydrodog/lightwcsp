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
#include "convert.hh"

using namespace std;

// http://www.linuxhowtos.org/C_C++/socket.htm

#define PORT "2000"
#define BACKLOG 10
#define BUFFSIZE 1024
#define VARSIZE 32

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

unsigned int numbersize(int n)
{
	if(n<0)
		n *= -1;
	if(n<10)
		return 1;
	else if(n<100)
		return 2;
	else if(n<1000)
		return 3;
	else
		return 3*numbersize(n/1000);
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
	if(read(sockfd,buff,BUFFSIZE - 1) < 0)
		error_die("Error reading from socket");
	if(ch)
	{
		struct stat finfo;

		if(argc < 4)
			error_die("No input file");

		int infile = open(argv[3],O_RDONLY);
		int outfile = open("output.dat",O_WRONLY);
		
		if(!infile)
			error_die("Error opening input file");

		else if(fstat(infile,&finfo))
			error_die("Error getting file's info");

		else
		{
			char *var = new char[finfo.st_size+1];
			if(read(infile,var,finfo.st_size) < 0)
				error_die("Error reading input file");
			var[finfo.st_size] = '\0';

			int i,j;
			for(i = j = 0; var[i] != '-'; i++)
				if(var[i]=='\n')
					j++;
			for(; var[i] != '\n'; i++);
			
			char *text = var + i + 1;		// iterates through the text (after variables positions)

			i = 0;
			char *auxv = var;						// iterates through the variables positions
			char *auxb = buff;					// iterates through the received message (from server)
			char *auxw = new char[50];	// buffer to receive converted string
			for(int k = 0; k < j; k++)
			{
				sscanf(auxv,"%d",&i);
				auxv += numbersize(i) + 1;
				write(outfile,text,i);
				text += i;
				switch(*auxb++)
				{
					case 0:			// no type
						break;
					case 1:			// int
						i = convert(auxw,*((int*)auxb);
						write(outfile,auxw,i);
						auxb += sizeof(int);
						break;
					case 2:			// unsigned int
						i = convert(auxw,*((unsigned int*)auxb);
						write(outfile,auxw,i);
						auxb += sizeof(unsigned int);
						break;
					case 3:			// long
						i = convert(auxw,*((long*)auxb);
						write(outfile,auxw,i);
						auxb += sizeof(long);
						break;
					case 4:			// long long
						i = convert(auxw,*((long long*)auxb);
						write(outfile,auxw,i);
						auxb += sizeof(long long);
						break;
					case 5:			// unsigned long long
						i = convert(auxw,*((unsigned long long*)auxb);
						write(outfile,auxw,i);
						auxb += sizeof(unsigned long long);
						break;
					case 6:			// float
						i = convert(auxw,*((float*)auxb);
						write(outfile,auxw,i);
						auxb += sizeof(float);
						break;
					case 7:			// double
						i = convert(auxw,*((double*)auxb);
						write(outfile,auxw,i);
						auxb += sizeof(double);
						break;
					case 8:			// long double
						break;
					case 9:			// char
						break;
					default:
						error_die("Data type unknown");
				}
			}
			delete[] auxw;
			delete[] var;
		}
		close(infile);
		close(outfile);

	}
	else
	{
	}


	close(sockfd);

	freeaddrinfo(result);

	return 0;
}