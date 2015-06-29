#include <iostream>
#include "convert.hh"
using namespace std;

#define pr 100


int main(int argc, char* argv[])
{
	int n = atoi(argv[1]);
	// double d = 0.1;
	// char *s;
	// for(int i=0;i<n;i++)
	// 	s = convert((d+=(0.1*i)),pr);
	// return 0;
	long long d = 1;
	char *s = new char[21*n];
	char *p = s;
	for(int i=0;i<n;i++)
	{
		int j = convert(&s,d*=-10);
		// cout << j << ' ' << d << ' ' << s << endl;
		p += j;
	}
	delete[] s;
	return 0;
}