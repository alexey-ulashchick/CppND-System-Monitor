#include <unistd.h>
#include <algorithm>
#include <cstddef>
#include <set>
#include <string>
#include <unordered_set>
#include <vector>
#include <iostream>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

void System::Update() {
  operatingSystem = LinuxParser::OperatingSystem();
  kernel = LinuxParser::Kernel();
  memoryUtilization = LinuxParser::MemoryUtilization();
  runningProcesses = LinuxParser::RunningProcesses();
  upTime = LinuxParser::UpTime();

  processor.Update();
  System::UpdateProcesses();
}

void System::UpdateProcesses() {
  auto pids = LinuxParser::Pids();
  std::unordered_set<int> pidsSet(pids.begin(), pids.end());

  auto it = processes.begin();
  while (it != processes.end()) {
    if (pidsSet.find(it->first) == pidsSet.end()) {
      it = processes.erase(it);
    } else {
      it++;
    }
  }

  auto users = LinuxParser::GetAllUsers();
  auto hz = sysconf(_SC_CLK_TCK);

  for (const int& pid : pidsSet) {
    auto totalTime = LinuxParser::GetProcessState(pid).TotalTime();

    if (processes.find(pid) == processes.end()) {
      auto uid = LinuxParser::Uid(pid);
      auto it = users.find(uid);
      auto userName = (it == users.cend()) ? "N/A" : it->second;
      processes[pid] = Process(pid, userName, hz, totalTime, upTime);
    } else {
      processes.at(pid).Update(totalTime, upTime);
    }
  }
}

std::string System::GetOperatingSystem() { return operatingSystem; }
std::string System::GetKernel() { return kernel; }
Processor& System::GetProcessor() { return processor; }
float System::GetMemoryUtilization() { return memoryUtilization; }
int System::GetTotalProcesses() { return processes.size(); }
int System::GetRunningProcesses() { return runningProcesses; }
long long System::GetUpTime() { return LinuxParser::UpTime(); }
vector<Process> System::GetProcesses() {
  vector<Process> result;

  for (auto& it : processes) {
    result.emplace_back(it.second);
  }

  std::sort(result.begin(), result.end());

  return result;
}
