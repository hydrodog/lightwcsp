#include <iostream>
using namespace std;
#include "convert.hh"

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
	char *s = new char[23];
	for(int i=0;i<n;i++)
	{
		int j = convert(&s,d*=-1000);
		cout << j << ' ' << d << ' ' << s << endl;
	}
	delete[] s;
	return 0;
}