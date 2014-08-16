/*
 * Config.cc
 *
 *  Created on: May 22, 2014
 *      Author: dkruger
 */
#include "include/Config.h"
#include "File.h"
#include "Buffer.h"

Config::Config() {
	Buffer buf;
	File::readFully("conf/lightwcsp.conf", buf);

	/*while (!conf.eof()) {
		conf.getline(buf, sizeof(buf));
		int sep = 0;
		int firstLen, secondInd, secondLen;
		for (sep = 0; sep < sizeof(buf); sep++){
			if (buf[sep] == '\t') {
				buf[sep]
				stringstream s(buf);
				s >> sym >> val;
			}
		}
	}*/
}

void Config::load() {
    
}

