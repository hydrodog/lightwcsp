#ifndef BUFFER_HH_
#define BUFFER_HH_

#include <string>
#include <string.h>
#include <stdio.h>

/*
 * String buffer with methods for efficiently printing
 * values into the buffer.
 *
 */
class Buffer {
private:
	int size;
	int length;
	char * data;
public:
	Buffer(int initialSize = 32768);
	Buffer(int initialSize, const char * initialValue);
	~Buffer(){
		flush();
		delete[] data;
		
	}
	void flush(){
		
	}	
	void append(const char * right){
		append(right, (int)strlen(right));
	}
	void append(const char * right, int bytes){
		checkSpace(bytes);
		length += sprintf(data+length,"%s",right);
	}
	void append(int v){
		checkSpace(10);
		length += sprintf(data+length,"%d",v);
	}
	void append(long long v){
		checkSpace(20);
		length += sprintf(data+length,"%lld",v);
	}
	void append(float v){
		checkSpace(10);
		length += sprintf(data+length,"%f",v);
	}
	void append(double v){
		checkSpace(20);
		length += sprintf(data+length,"%lf",v);
	}
	void append(char v){
		checkSpace(1);
		data[length++] = v;
	}
	void append(double v[],int number,const std::string& sep);
	void checkSpace(int bytes) {
		int newLength = length + bytes;
		if (newLength >= size) {
			size = newLength << 1;
			char * tmp = new char[size];
			memcpy(tmp, data, length);
			data = tmp;
		}
	}
	int getSize(){
		return size;
	}
	int getLength(){
		return length;
		}
	const char * c_str(){
		return data;
	}
};

#endif /*Buffer_HH */
