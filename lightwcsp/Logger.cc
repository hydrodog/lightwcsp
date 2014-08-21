/*
 * Logger.cc
 *
 *  Created on: May 22, 2014
 *      Author: dkruger
 */
#include "Logger.h"

Logger::Logger() {
	debugLevel = 5;
	fp = nullptr;
}

Logger::Logger(int level) {
	debugLevel = level;
	fp = nullptr;
}

void Logger::loadConfig() {
  //log.open(config.getString("filename"));
  //setLevel(config.getInt("debuglevel"));
}



