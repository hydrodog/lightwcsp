#include <iostream>
#include "convert.hh"
using namespace std;

#define pr 100


int main(int argc, char* argv[])
{
	// double d = 0.1;
	// char *s;
	// for(int i=0;i<n;i++)
	// 	s = convert((d+=(0.1*i)),pr);
	// return 0;
	char *s = new char[50];
	int i = convert(&s,-35434567.123241512323,5);
	cout << s << ' ' << i << endl;
	delete[] s;
	return 0;
}