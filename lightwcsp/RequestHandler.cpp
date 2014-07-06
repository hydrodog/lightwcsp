/*
 * RequestHandler.cpp
 *
 *  Created on: Jun 21, 2014
 *      Author: AndresRicardo
 */

#include "RequestHandler.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <cstring>
#include <ctype.h>

char * RequestHandler::getNextToken
(int &cursor, char * data, int &dataSize,
 const int socketId, int &tokenLength) {
  int size = 100;
  int length = 0;
  char * result = new char[size + 2];
  while (isspace(data[cursor])) {
    cursor++;
  }
  while (!isspace(data[cursor])) {
    if (length > size) {
      size = length << 1;
      char * tmp = new char[size + 2];
      memcpy(tmp, result, length);
      result = tmp;
    }
    result[length] = data[cursor];
    cursor++;
    length++;
    if (cursor == dataSize) {
      dataSize = recv(socketId, data, BUFFER_SIZE, 0);
      cursor = 0;
    }
  }
  result[length] = 0;
  tokenLength = length;
  return result;
}

RequestHandler::RequestHandler(const int socketId) {
  char data[BUFFER_SIZE];
  int dataSize = 0;
  int currentTokenLength = 0;
  int cursor = 0;
  dataSize = recv(socketId, data, BUFFER_SIZE, 0);
  char * currentToken = getNextToken(cursor, data, dataSize, socketId,
				     currentTokenLength);
  if (strcmp(currentToken, "POST") == 0) {
    method = POST;
  } else if (strcmp(currentToken, "GET") == 0) {
    method = GET;
  } else {
    method = UNIMPLEMENTED;
  }
  url = getNextToken(cursor, data, dataSize, socketId, currentTokenLength);
  urlLength = currentTokenLength;
  httpVersion = getNextToken(cursor, data, dataSize, socketId,
			     currentTokenLength);
  getNextToken(cursor, data, dataSize, socketId, currentTokenLength);
  host = getNextToken(cursor, data, dataSize, socketId, currentTokenLength);
  body = getNextToken(cursor, data, dataSize, socketId, currentTokenLength);
  bodySize = sizeof(body);
  bodyLength = currentTokenLength;
}
