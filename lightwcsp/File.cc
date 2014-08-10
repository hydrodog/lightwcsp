#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "File.h"

int File::readFully(const char filename[], char *p) {
	int fh = open(filename, O_RDONLY);
	if (fh < 0)
		return NULL;
	struct stat s;
	fstat(fh, &s);
	//char* p = new char[s.st_size];
	int bytesRead = read(fh, p, s.st_size);
	if (bytesRead < s.st_size) {
		//delete [] p;
		return -1;
	}
	return bytesRead;
}

int File::readFully(const char filename[], Buffer &buf) {
	int fh = open(filename, O_RDONLY);
	if (fh < 0)
		return NULL;
	struct stat s;
	fstat(fh, &s);
	char* p = new char[s.st_size];
	int bytesRead = read(fh, p, s.st_size);
	if (bytesRead < s.st_size) {
		delete[] p;
		return -1;
	}
	buf.append(p, bytesRead);
	return bytesRead;
}

int readFully(const char filename[], char* p, int size) {
	int fh = open(filename, O_RDONLY);
	if (fh < 0)
		return NULL;
	int bytesRead = read(fh, p, size);
	if (bytesRead < size) {
		delete[] p;
		return -1;
	}
	return bytesRead;
}
