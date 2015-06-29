#include <iostream>
#include <cstdlib>
#include <cmath>
#include "ecvt.c"

#define pr 100

using namespace std;

int main(int argc, char* argv[])
{
	int n = atoi(argv[1]);
	double d = 1.1;
	int dec, sign;
	char *s = ecvt(0.234,5,&dec,&sign);
	cout << s << endl;
	// for(int i=0;i<n;i++)
	// {
	// 	int aux = ceil(log10(floor(d)));
	// 	s = ecvt((d+=(0.1*i)),aux+2,&dec,&sign);
	// }
	return 0;
}