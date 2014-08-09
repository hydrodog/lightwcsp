#ifndef FILE_H__
#define FILE_H__
#include "Buffer.h"

class File {
public:
	/**
	 * Open a file, and read it fully into a Buffer
	 * appending to whatever is already there
	 */
	static int readFully(const char filename[], Buffer& buf);

	/**
	 * Open a file, and read it fully into a char*
	 * and return the number of bytes read.
	 */
	static int readFully(const char filename[], char* p);

	/**
	 * Open a file, and read it fully into a char*
	 * and return the number of bytes read.
	 */
	static int readFully(const char filename[], char* p, int size);

	// write a buffer out to a file
	void write(const char filename[], const Buffer& buf);
};

#endif
