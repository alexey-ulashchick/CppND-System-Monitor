#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <unordered_map>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

class System {
 public:
  /** Updates internal state of the object. */
  void Update();

  std::string GetOperatingSystem();
  std::string GetKernel();
  Processor& GetProcessor();
  float GetMemoryUtilization();
  int GetTotalProcesses();
  int GetRunningProcesses();
  long long GetUpTime();

  std::vector<Process> GetProcesses();  // TODO: See src/system.cpp

 private:
  void UpdateProcesses();
  std::string operatingSystem = "";
  std::string kernel = "";
  Processor processor = Processor();
  float memoryUtilization = 0;
  int runningProcesses = 0;
  long long upTime = 0;

  std::unordered_map<int, Process> processes{};
};

#endif