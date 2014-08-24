/*
 * RequestHandler.h
 *
 *  Created on: Jun 21, 2014
 *      Author: AndresRicardo
 */

#ifndef REQUESTHANDLER_H_
#define REQUESTHANDLER_H_

 #include <map>

enum httpMethod {
	GET, POST, HEAD, PUT, DELETE, OPTIONS, CONNECT, UNIMPLEMENTED
};

class RequestHandler {
private:
	std::map<const char*, char*> properties;
	const int BUFFER_SIZE = 1024;
	int bodySize;
	int bodyLength;
	int urlLength;
	httpMethod method;
	char * url;
	char * httpVersion;
	char * host;
	char * body;
	char * getNextToken(int &cursor, char * data, int &dataSize,
			const int socketId, int &tokenLength);
	char * getNextHeader(int &cursor, char * data, int &dataSize,
			const int socketId, int &tokenLength);
public:
	RequestHandler(int socketId);
	inline int getBodyLength() const {
		return bodyLength;
	}
	inline int getUrlLength() const {
		return urlLength;
	}
	inline const char * getBody() const {
		return body;
	}
	inline const char * getVersion() const {
		return httpVersion;
	}
	inline const char * getHost() const {
		return host;
	}
	inline const char * getUrl() const {
		return url;
	}
	inline const httpMethod getMethod() const {
		return method;
	}
	void printProperties();

	inline bool hasProperty(const char* property) {
		return (properties.find(property)!=properties.end());
	}

	inline char* getProperty (const char* property) {
		std::map<const char*, char*>::iterator it = properties.find(property);
		if(it!=properties.end()) {
			return it->second;
		}
		else {
			return nullptr;
		}
	}
};

#endif /* REQUESTHANDLER_H_ */
