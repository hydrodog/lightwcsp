#ifndef FILE_H__
#define FILE_H__

class Buffer;
class File {
public:
	/**
	 * Open a file, and read it fully into a Buffer
	 * appending to whatever is already there
	 */
	static int readFully(const char filename[], Buffer& buf);

	/**
	 * Open a file, and read it fully into a Buffer
	 * and return a pointer to said buffer
	 */
	static int readFully(const char filename[], char* p);

	// write a buffer out to a file
	void write(const char filename[], const Buffer& buf);
};

#endif
