/*
 * RequestHandler.cpp
 *
 *  Created on: Jun 21, 2014
 *      Author: AndresRicardo
 */

#include <HttpRequest.hh>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <cstring>
#include <string>
#include <CspServlet.hh>
//#include <server.hh>
using namespace std;
#define BASEDIR "htdocs"
//void *__gxx_personality_v0;
FileSys HttpRequest::FS(BASEDIR);

const char * HttpRequest::getNextToken(int &cursor, int &tokenLength) {
	while (isBlank(data[cursor])) {
	cursor++;
	}
	int start = cursor;
	while (!isBlank(data[cursor])) {
	cursor++;
	if (cursor >= BUFFER_SIZE) {
		//TODO: can't handle multiple buffers, 
		//but buffer is now 4200 bytes
		//preallocate max size and be done
		throw "Fix error: can't handle huge inputs";
		// dataSize = recv(socketId, data, BUFFER_SIZE, 0);
		//cursor = 0;
	}
	}
	tokenLength = cursor - start;
	return data+start;
}
void HttpRequest::tokenlize(){
	int currentTokenLength = 0;
	int cursor = 0;
	const char * currentToken = getNextToken(cursor, currentTokenLength);
	if (strncmp(currentToken, "POST", 4) == 0) {
	method = POST;
	} 
	else if (strncmp(currentToken, "GET", 3) == 0) {
	method = GET;
	} 
	else {
	method = UNIMPLEMENTED;
	}
	url = getNextToken(cursor, currentTokenLength);
	urlLength = currentTokenLength;
	httpVersion = getNextToken(cursor, currentTokenLength);
	getNextToken(cursor, currentTokenLength);
	host = getNextToken(cursor, currentTokenLength);
	body = getNextToken(cursor, currentTokenLength);
	bodySize = sizeof(body);
	bodyLength = currentTokenLength;
}

void HttpRequest::handler(){
	string temp= BASEDIR;
	cout<<"url: " <<url<<endl;
	if (url[urlLength-1]=='/')
	temp+= "/index.html";
	else temp+= string(url, urlLength);
	HttpServlet* s = FS.access(temp); //TODO: eliminate string

	if (s == nullptr) {
	cerr << "not find key "<< url << std::endl;
		return;
	}
	s->doGet(*this);
}

HttpRequest::HttpRequest(const int socketId) : socketId(socketId) {
}

void HttpRequest::run(){
	read();
	tokenlize();
	handler();
}

void HttpsRequest::read(){
		SSL_library_init();
		dataSize = SSL_read(ssl, data, BUFFER_SIZE);
		if (dataSize>0){
			data[dataSize]=0;
			cout<<"Client MSG:"<<data<<endl;
			url = data;
			urlLength = dataSize;
		}
		else {
			cout<<"didn`t receive data"<<endl;
			ERR_print_errors_fp(stderr);
		}
}

HttpsRequest::HttpsRequest(SSL* ssl, const int socketId):HttpRequest(socketId),ssl(ssl){
	if ( SSL_accept(ssl) == -1 )     /* do SSL-protocol accept */
        ERR_print_errors_fp(stderr);
	else 
	{
		read();
		handler();
	}
}

HttpsRequest::~HttpsRequest(){
	cout<<"delete HttpsRequest"<<endl;
}