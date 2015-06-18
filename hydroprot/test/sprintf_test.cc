#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
	int n = atoi(argv[1]);
	char* buf = new char[n * 10];
	char*p = buf;
	for (int i = 0; i < n; i++) {
		int bytes = sprintf(p, "%d", i);
		p += bytes;
	}
	return 0;
}
		
