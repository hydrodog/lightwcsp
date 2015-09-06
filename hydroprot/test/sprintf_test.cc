#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
	int n = atoi(argv[1]);
	long long d = 1;
	char* buf = new char[n * 20];
	char*p = buf;
	for (int i = 0; i < n; i++) {
		int bytes = sprintf(p, "%lli", (d*=-10));
		p += bytes;
	}
	delete[] buf;
	return 0;
}
		
