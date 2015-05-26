#include "Bench.hh"
#include <unistd.h>
using namespace std;

Bench::Bench() :
  elapsedUserTime(0), elapsedSystemTime(0), elapsedWallTime(0),
  BENCHCLOCKS_PER_SEC(sysconf(_SC_CLK_TCK)) {
}


void Bench::start() {
  times(&startCPUTimes);
  time(&startWallTime);
}

void Bench::stop() {
  struct tms endCPUTimes;
  time_t endWallTime;
  times(&endCPUTimes);
  time(&endWallTime);
  elapsedUserTime += endCPUTimes.tms_utime - startCPUTimes.tms_utime;
  elapsedSystemTime += endCPUTimes.tms_stime - startCPUTimes.tms_stime;
  elapsedWallTime += endWallTime - startWallTime;
}

ostream& operator <<(ostream& s, const Bench& b) {
  return
    s <<
    "User:   " << b.elapsedUserTime / double(b.BENCHCLOCKS_PER_SEC) << '\n' <<
    "System: " << b.elapsedSystemTime  / double(b.BENCHCLOCKS_PER_SEC) << '\n' <<
    "Wall:   " << b.elapsedWallTime << '\n';
}
