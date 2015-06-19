#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
	int n = atoi(argv[1]);
	double d = 0.1;
	char* buf = new char[n * 100];
	char*p = buf;
	for (int i = 0; i < n; i++) {
		int bytes = sprintf(p, "%lf", (d+=(0.1*i)));
		p += bytes;
	}
	delete[] buf;
	return 0;
}
		
