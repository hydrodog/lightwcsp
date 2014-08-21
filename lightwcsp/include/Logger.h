/*
 * Logger.h
 *
 *  Created on: May 22, 2014
 *      Author: dkruger
 */

#ifndef LOGGER_H_
#define LOGGER_H_
#include <string>
#include <vector>
#include <cstdio>
#include <stdlib.h>
#include <iostream>

class Logger {
private:
	int debugLevel; // the higher it is, the more messages displayed
	std::vector<std::string> messages;
	FILE* fp; // I hate old style C files, but fprintf is really convenient.
	// we can do better later.
public:
	enum LEVELS {
		FATAL = 0, // extremely bad, instant death
		ERROR,     // bad, a serious error
		WARNING,   // something to watch out for
		INFO,      // important to know
		DETAILS    // detailed information for tracking down bugs
	};
	enum MESSAGE_CODES {
		RESOURCE_NOT_FOUND = 0, // the first group are errors
		FILE_NOT_FOUND,
		PERMISSION_DENIED,
		METHOD_NOT_IMPLEMENTED,
		OK,
		DOV_SAYS_NO,
		YOU_MUST_BE_JOKING,
		LISTEN_FAILED,
		SIGACTION_FAILED,
		ACCEPT_FAILED,
		GET_ADDR_INFO,
		CANNOT_OPEN_SOCKET,
		SETSOCKOPT,
		BIND,
		FAILED_TO_BIND,
		SIG_ACTION,

		EXHAUSTED_THREADS,

		RUNNING,
		SERVER_OK,
		LOGGER_STARTED,
		CREATING_THREADPOOL,
		ABOUT_TO_ACCEPT,
		ACCEPTED_SOCKET,
		STARTED_THREAD,
		ENDED_THREAD

	};

	Logger();

	Logger(int level);

	void loadConfig();

	void message(int level, const int MSGCODE) {
		if (debugLevel >= level)
			fprintf(fp, messages[MSGCODE].c_str());
	}
	void message(int level, const int MSGCODE, int param) {
		if (debugLevel >= level)
			fprintf(fp, messages[MSGCODE].c_str(), param);
	}
	void message(int level, const int MSGCODE, int param1, int param2) {
		if (debugLevel >= level)
			fprintf(fp, messages[MSGCODE].c_str(), param1, param2);
	}
	void fatal(int MSGCODE) {
		fprintf(fp, messages[MSGCODE].c_str());
		exit(1); // use throw instead?
	}

};
#endif /* LOGGER_H_ */
