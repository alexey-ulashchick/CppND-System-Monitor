#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

void Process::Update(const long long& nextTotalTime,
                     const long long& nextUpTime) {
  float nextTotalSeconds = nextTotalTime / hz;
  float prevTotalSeconds = prevTotalTime / hz;
  long long dUpTime = nextUpTime - prveUpTime;

  utilization = dUpTime == 0 ? 0 : (nextTotalSeconds-prevTotalSeconds) / dUpTime;

  prevTotalTime = nextTotalTime;
  prveUpTime = nextUpTime;
};

int Process::GetPid() { return pid; }
string Process::GetUser() { return user; }
float Process::GetCpuUtilization() { return utilization; }

// TODO: Return the command that generated this process
string Process::Command() { return string(); }

// TODO: Return this process's memory utilization
string Process::Ram() { return string(); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return 0; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
  return a.utilization < utilization;
}