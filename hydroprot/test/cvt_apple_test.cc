#include <iostream>
#include <cstdlib>
#include "cvt_apple.c"

#define pr 100

using namespace std;

int main(int argc, char* argv[])
{
	int n = atoi(argv[1]);
	double d = 0.1;
	int dec, sign;
	char *s;
	for(int i=0;i<n;i++)
		s = cvt((d+=(0.1*i)),pr,&dec,&sign,1);
	return 0;
}