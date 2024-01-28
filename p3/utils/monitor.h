#ifndef MONITOR
#define MONITOR

#include <mutex>
#include <condition_variable>
#include "Semaphore_V4.hpp"
#include "tiempo.h"
#include <chrono>

using namespace chrono;
using namespace std;

class Monitor
{
public:
  Monitor(int nTotal, int nCompletados, bool mostrarProgreso, std::chrono::_V2::system_clock::time_point _start_time);
  void increaseN();

private:
  int n, nTotal, previousProgress;
  bool mostrarProgreso;
  std::chrono::_V2::system_clock::time_point start_time;

  mutex mtxMonitor;
};

#endif