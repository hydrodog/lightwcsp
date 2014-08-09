/*
 * Config.h
 *
 *  Created on: May 22, 2014
 *      Author: dkruger
 */

#ifndef CONFIG_H_
#define CONFIG_H_
#include <iostream>
#include <map>
#include <stdlib.h>
class Config {
private:
	std::map<const char*, void*> properties;
	std::map<int, std::string> standardMessages;
public:
	/*
	 * The constructor does nothing, all the action is in load
	 * This is not a great idea, but necessary for the timing to be
	 * controlled in main
	 */
	Config();

	// load is separate from constructor to control timing, happens in main().
	void load();

	int getInt(const char* key) {
		void* val = properties[key];
		std::cout << key << ": " << val << "\n";
		return 0;
	}
	int getInt(const char* key, int defaultVal) {
		void* val = properties[key];
		std::cout << key << ": " << val << "\n";
		if (val == NULL)
			return defaultVal;
		return atoi((char*) val);
	}

	const char* getString(const char* key) {
		void* val = properties[key];
		std::cout << key << ": " << val << "\n";
		return (const char*) val;
	}

	inline const std::string getStandardMessage(int code) {
		return standardMessages[code];
	}

	inline const int getStandardMessageLength(int code) {
		return standardMessages[code].length();
	}

};

#endif /* CONFIG_H_ */
