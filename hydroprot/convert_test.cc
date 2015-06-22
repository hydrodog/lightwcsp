#include <iostream>
#include "convert.c"

#define pr 100

using namespace std;

int main(int argc, char* argv[])
{
	int n = atoi(argv[1]);
	// double d = 0.1;
	// char *s;
	// for(int i=0;i<n;i++)
	// 	s = convert((d+=(0.1*i)),pr);
	// return 0;
	int d = 1;
	char *s;
	for(int i=0;i<n;i++)
		s = itoa(d+=(-10*i));
	return 0;
}