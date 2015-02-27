/*
 * MySringBuffer.cpp
 *
 *  Created on: Jun 21, 2014
 *      Author: AndresRicardo
 */

#include "MyStringBuffer.h"
#include <cstring>

MyStringBuffer::MyStringBuffer(int initialSize) {
	data = new char[initialSize];
	data[0] = 0;
	size = initialSize;
	length = 0;
}

MyStringBuffer::MyStringBuffer(int initialSize, const char * initialValue) {
	data = new char[initialSize];
	size = initialSize;
	length = 0;
	do {
		data[length] = initialValue[length];
	} while (initialValue[length++] != 0);
}

void inline MyStringBuffer::append(const char * right) {
	this->append(right, strlen(right));
}

void MyStringBuffer::append(const char * right, int bytes) {
	int newLength = length + bytes;
	if (newLength > size - 1) {
		size = newLength << 1;
		char * tmp = new char[size];
		memcpy(tmp, data, length);
		data = tmp;
	}
	memcpy(data + length, right, bytes);
	data[newLength] = 0;
	length = newLength;
}

int inline MyStringBuffer::getSize() {
	return size;
}

int inline MyStringBuffer::getLength() {
	return length;
}

const char * MyStringBuffer::c_str() {
	return data;
}

