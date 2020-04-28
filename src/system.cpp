#include <unistd.h>
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <set>
#include <string>
#include <unordered_set>
#include <vector>

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

  // Clean up list of stale processes.
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

  // Creates or updates processes.
  for (const int& pid : pidsSet) {
    auto processState = LinuxParser::GetProcessState(pid);

    if (processes.find(pid) == processes.end()) {
      auto uid = LinuxParser::Uid(pid);
      auto command = LinuxParser::Command(pid);
      auto ram = LinuxParser::Ram(pid);
      auto it = users.find(uid);
      auto userName = (it == users.cend()) ? "N/A" : it->second;
      processes[pid] =
          Process(pid, userName, command, ram, processState.starttime, hz,
                  processState.TotalTime(), upTime);
    } else {
      processes.at(pid).Update(processState.TotalTime(), upTime,
                               processState.starttime);
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
