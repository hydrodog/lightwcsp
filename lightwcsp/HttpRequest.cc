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

//#include <server.h>
using namespace std;

//void *__gxx_personality_v0;
FileSys HttpRequest::FS("htdocs");

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

HttpRequest::HttpRequest(const int socketId,
												 const char baseDir[]) : socketId(socketId),
  baseDir(baseDir), baseDirLen(strlen(baseDir)) {
  int currentTokenLength = 0;
  int dataSize = recv(socketId, data, BUFFER_SIZE, 0);
  int cursor = 0;
  const char * currentToken = getNextToken(cursor, currentTokenLength);
  if (strncmp(currentToken, "POST", 4) == 0) {
    method = POST;
  } else if (strncmp(currentToken, "GET", 3) == 0) {
    method = GET;
  } else {
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

  HttpServlet* s = FS.access(string(url, urlLength)); //TODO: eliminate string
  
  if (s == nullptr) {
    cerr << "not find key "<< url << endl;
		return;
  }
	
	s->doGet(*this);

}
#if 0
		send(socketId, f->getBuf(), f->getFileLen(), 0);
    if (f->filetype == "csp"){
      HttpRequest req;
      HttpServlet::dispatcher(fl->filename, req);
      headers(client,path);
      send(client, req.getOutput().c_str(), req.getOutput().getLength(), 0);
      send(client, "\r\n", 2, 0);
    }
    else{
      headers(client,path);
      send(client, fs->get_buf(fl), fs->get_filelen(fl), 0);
      send(client, "\r\n", 2, 0);
    }
  }
#endif


