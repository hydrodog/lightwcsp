#ifndef BENCH_HH_
#define BENCH_HH_

#include <iostream>
#include <sys/times.h>
#include <time.h>

class Bench {
 private:
	const long BENCHCLOCKS_PER_SEC;
   struct tms startCPUTimes;
   time_t     startWallTime;
   long long elapsedUserTime;
   long long elapsedSystemTime;
   long long elapsedWallTime;
 public:
	Bench();
   void start();
   void stop();
   friend std::ostream& operator <<(std::ostream& s, const Bench& b);
};
#endif
