#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <sys/times.h>
#include "Signal.hh"

using namespace std;

struct tms timesBuf;
struct tms lastBuf;

void signalHandler(int signum) {
  clock_t wallTime = times(&timesBuf);
  clock_t diffU = timesBuf.tms_utime - lastBuf.tms_utime;
  clock_t diffS = timesBuf.tms_stime - lastBuf.tms_stime;
  lastBuf = timesBuf;
  cerr << diffU << "\t" << diffS << '\n';
}

void setupSignal() {
  clock_t wallTime = times(&lastBuf);
  if (signal(SIGUSR1, signalHandler) == SIG_ERR)
    cerr << "signal did not set properly\n";
}
