/*
 * Buffer.cpp
 *
 *  Created on: March 22, 2015
 *      Author: Dov Kruger
 */

#include "Buffer.hh"
#include <cstring>

Buffer::Buffer(int initialSize) {
	data = new char[initialSize];
	data[0] = 0;
	size = initialSize;
	length = 0;
}

Buffer::Buffer(int initialSize, const char * initialValue) {
	data = new char[initialSize];
	size = initialSize;
	length = 0;
	do {
		data[length] = initialValue[length];
	} while (initialValue[length++] != 0);
}

#if  0
/*
 * Create Buffer initialized with http headers with embedded mime type
 */
char* Buffer::Buffer(size_t initialsize, const char* header, size_t size,
										 const string& mimeType) {
	data = new char[initialSize];
	size = initialSize;	
  char* buf = data;
	memcpy(buf, header, size);
	buf += size;
	memcpy(buf, mimeType.c_str(), mimeType.length());
	buf += mimeType.length();
	memcpy(buf, "\r\n\r\n", 4);
  length = buf - data;
}
#endif

void Buffer::append(double v[],int number,const std::string& sep){
	checkSpace(number*(20+sep.length()));
	for (int i = 0;i < number-1;i++){
		length += sprintf(data+length,"%lf%s",v[i],sep.c_str());		
	}
}
