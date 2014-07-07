#include "Buffer.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <memory.h>
#include <File.h>

using namespace std;

inline void Buffer::checkSpace(int newSize) {
	if (used + newSize > size) {
		const char* temp = buffer;
		buffer = new char[size + size]; // allocate twice the size
		memcpy(buffer, temp, used);
		size = newSize;
	}
}

void Buffer::flush() {

}

void Buffer::load(const char filename[]) {
	File::readFully(filename, buffer, size);
}

void Buffer::append(const char buf[], int n) {
	checkSpace(n);
	memcpy(buffer + used, buf, n);
	used += n;
}

void Buffer::append(const std::string& s) {
	checkSpace(s.length());
	memcpy(buffer + used, s.c_str(), s.length());
	used += s.length();
}

enum {
	INT_PRINTABLE_SIZE = 11,   // ex -1234567890
	FLOAT_PRINTABLE_SIZE = 13, // ex -1.234567e+23
	DOUBLE_PRINTABLE_SIZE = 21 // ex -1.23456789012345e+23
};

void Buffer::append(int val) {
	checkSpace(INT_PRINTABLE_SIZE);
	int len = sprintf(buffer + used, "%d", val);
	used += len;
}
void Buffer::append(float val) {
	checkSpace(FLOAT_PRINTABLE_SIZE);
	int len = sprintf(buffer + used, "%f", val);
	used += len;
}

void Buffer::append(double val) {
	checkSpace(DOUBLE_PRINTABLE_SIZE);
	int len = sprintf(buffer + used, "%lf", val);
	used += len;
}

void Buffer::append(int vals[], int n, const char sep[]) {
	if (n <= 0)
		return;
	int seplen = strlen(sep);
	checkSpace(n * (INT_PRINTABLE_SIZE + seplen));
	int totalLen = 0;

	Node* p = buffer + used;
	for (int i = 0; i < n; i++) {
		int len = sprintf(p, "%d", val);
		totalLen += len;
		p += len;
	}
	used += totalLen;
}

void Buffer::append(float vals[], int n, const char sep[]) {
	if (n <= 0)
		return;
	int seplen = strlen(sep);
	checkSpace(n * (FLOAT_PRINTABLE_SIZE + seplen));
	int totalLen = 0;

	Node* p = buffer + used;
	for (int i = 0; i < n; i++) {
		int len = sprintf(p, "%d", val);
		totalLen += len;
		p += len;
	}
	used += totalLen;
}

void Buffer::append(double vals[], int n, const char sep[]) {
	if (n <= 0)
		return;
	int seplen = strlen(sep);
	checkSpace(n * (DOUBLE_PRINTABLE_SIZE + seplen));
	int totalLen = 0;

	Node* p = buffer + used;
	for (int i = 0; i < n; i++) {
		int len = sprintf(p, "%d", val);
		totalLen += len;
		p += len;
	}
	used += totalLen;
}

// copy this buffer to another one, adding in dynamic content
void Buffer::writeToBuffer(Buffer& buf, DynList& dynlist) {
}

