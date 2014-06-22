/*
 * MyStringBuffer.h
 *
 *  Created on: June 21, 2014
 *      Author: Andres Contreras
 */

#ifndef MyStringBuffer_H_
#define MyStringBuffer_H_
#define DEFAULT_BUFFER_SIZE 4096

class MyStringBuffer {
private:
	int size;
	int length;
	char * data;
public:
	MyStringBuffer(int initialSize = DEFAULT_BUFFER_SIZE);
	MyStringBuffer(int initialSize, const char * initialValue);
	void inline append(const char * right);
	void append(const char * right, int bytes);
	int getSize();
	int getLength();
	const char * c_str();
};

#endif /* MyStringBuffer_H_ */
