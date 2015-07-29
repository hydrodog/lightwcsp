/*
 * Represent each HttpRequest as a single object containing the input
 * packet and methods to get parameters, find the connection back
 * to the client, anything to do with the connection.
 *
 *  Created on: Jun 21, 2014
 *      Author: AndresRicardo
 *  Dov Kruger: merged Andres' version with Jungyang's 4/2/2015
 * 
 */

#ifndef HTTP_REQUEST_HH_
#define HTTP_REQUEST_HH_
#include <Buffer.hh>
#include <FileSys.hh>
#include <sys/socket.h>
#include "openssl/ssl.h"
typedef struct ssl_st SSL;

enum httpMethod {
	GET, POST, HEAD, PUT, DELETE, OPTIONS, CONNECT, UNIMPLEMENTED
};

class HttpRequest {
protected:
	static FileSys FS;
	int socketId;
	Buffer b;           // every request has a buffer to write back to
  const char* baseDir;
  int baseDirLen;
	int dataSize;
  const static int BUFFER_SIZE = 8192;
  char data[BUFFER_SIZE]; // buffer for incoming request (static size)
	                        //8k is probably enough for most incoming
	//TODO: we don't currently handle longer incoming requests
  int bodySize;
  int bodyLength;
  int urlLength;
  httpMethod method;
  const char * url;
  const char * httpVersion;
  const char * host;
  const char * body;
  bool isBlank(const char currentChar) const {
    return
      currentChar == '\r' ||
      currentChar == '\n' ||
      currentChar == ' '	||
      currentChar == '\t';
  }
  const char* getNextToken(int &cursor, int &tokenLength);
public:
  HttpRequest(const char baseDir[], int client) : baseDir(baseDir), socketId(client) {}
	void parse();
  int getBodyLength() const { return bodyLength; }
  int getUrlLength() const { return urlLength; }
  const char * getBody() const { return body; }
  const char * getVersion() const { return httpVersion; }
  const char * getHost() const { return host; }
  const char * getUrl() const { return url; }
  const httpMethod getMethod() const { return method; }
	const char* getParam(const char* msg) const { return nullptr; }
	Buffer& getOutput(){
		return b;
	}
	virtual void read() {
		dataSize = recv(socketId, data, BUFFER_SIZE, 0);
	}
	virtual void send(const char* buf, size_t buflen);
};


class HttpsRequest:public HttpRequest{
	SSL *ssl;
public: 
	HttpsRequest(SSL* ssl, const int client);
	~HttpsRequest();
	int recv(){
		dataSize = SSL_read(ssl, data, BUFFER_SIZE);
	}
	void send(const char* buf, size_t buflen){
		SSL_write(ssl, buf, buflen);
	}
};

#endif //HTTP_REQUEST_HH_
