#include <iostream>
#include <cstdlib>
#include "ecvt.c"

#define pr 100

using namespace std;

int main(int argc, char* argv[])
{
	int n = atoi(argv[1]);
	double d = 0.1;
	int dec, sign;
	char *buf = new char[n*pr];
	char *s = buf;
	for(int i=0;i<n;i++)
	{
		s = ecvt((d+=(0.1*i)),pr,&dec,&sign);
		s += pr;
	}
	delete[] buf;
	return 0;
}