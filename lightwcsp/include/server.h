/*
 * server.h
 *
 *  Created on: Jun 21, 2014
 *      Author: AndresRicardo
 */

#ifndef SERVER_H_
#define SERVER_H_

#include <stdio.h> // Need for FILE
#include "Logger.h"

void accept_request(int client);
void not_found(int client);
void serve_file(int client, const char *filename, int fileSize);
int startup(u_short *port);
void unimplemented(int client);
void headers(int client, const char *filename, int fileSize);
void cat(int client, FILE *resource);
void error_die(const char *sc);
void sendStandardMessage(int client, Logger::MESSAGE_CODES messageType);

#endif /* SERVER_H_ */
