/*
 * RequestHandler.h
 *
 *  Created on: Jun 21, 2014
 *      Author: AndresRicardo
 */

#ifndef REQUESTHANDLER_H_
#define REQUESTHANDLER_H_

enum httpMethod {
	GET, POST, HEAD, PUT, DELETE, OPTIONS, CONNECT, UNIMPLEMENTED
};

class RequestHandler {
private:
	const int BUFFER_SIZE = 1024;
	int bodySize;
	int bodyLength;
	httpMethod method;
	char * url;
	char * httpVersion;
	char * host;
	char * body;
	bool isBlank(const char currentChar);
	char * getNextToken(int &cursor, char * data, int &dataSize,
			const int socketId, int &tokenLength);
public:
	RequestHandler(int socketId);
	int getBodyLength();
	const char * getBody();
	const char * getVersion();
	const char * getHost();
	const char * getUrl();
	const httpMethod getMethod();
};

#endif /* REQUESTHANDLER_H_ */
