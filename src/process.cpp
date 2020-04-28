#include <unistd.h>
#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::to_string;
using std::vector;

void Process::Update(const long long& nextTotalTime,
                     const long long& nextUpTime, const long long& startTime) {
  float nextTotalSeconds = nextTotalTime / hz;
  float prevTotalSeconds = prevTotalTime / hz;
  float dSeconds = nextTotalSeconds - prevTotalSeconds;
  long long dUpTime = nextUpTime - prveUpTime;

  utilization = dUpTime == 0 ? 0 : dSeconds / dUpTime;

  prevTotalTime = nextTotalTime;
  prveUpTime = nextUpTime;
  this->startTime = startTime;
};

const int& Process::GetPid() { return pid; }
const float& Process::GetCpuUtilization() { return utilization; }
long long Process::GetUpTime() { return prveUpTime - (startTime / hz); }
const std::string& Process::GetUser() { return user; }
const std::string& Process::GetCommand() { return command; }
const std::string& Process::GetRam() { return ram; }

bool Process::operator<(Process const& a) const {
  return a.utilization < utilization;
}