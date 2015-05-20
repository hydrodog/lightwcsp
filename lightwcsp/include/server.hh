/*
 * server.h
 *
 *  Created on: Jun 21, 2014
 *      Author: AndresRicardo
 */

#ifndef SERVER_H_
#define SERVER_H_

#define BASEDIR "htdocs"
#define SERVER_STRING "Server: lwc/1\r\n"
extern const char HEADER[];
extern const unsigned int HEADER_SIZE;

char* headers(char* buf, const std::string& mimeType);
//void cat(int client, FILE *resource);
void error_die(const char *sc);

#endif /* SERVER_H_ */
