/*
 * Logger.cc
 *
 *  Created on: May 22, 2014
 *      Author: dkruger
 */
#include "include/Logger.h"

Logger::Logger(){}

Logger::loadConfig() {
  log.open(config.getString("filename"));
  setLevel(config.getInt("debuglevel"));
}



