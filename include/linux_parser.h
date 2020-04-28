#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>
#include <unordered_map>

namespace LinuxParser {

struct ProcessorState {
  long nonIdle;
  long idle;
};

struct ProcessState {
  long utime;
  long stime;
  long cutime;
  long cstime;
  long long starttime;

  long long TotalTime();
};

// Paths
const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};

// System
std::string OperatingSystem();
std::string Kernel();

ProcessorState CpuUtilization();
float MemoryUtilization();

int TotalProcesses();
int RunningProcesses();
long long UpTime();

std::unordered_map<int, std::string> GetAllUsers();
std::vector<int> Pids();

// Processes
int Uid(int pid);
ProcessState GetProcessState(int pid);
std::string Command(int pid);
std::string Ram(int pid);
};  // namespace LinuxParser

#endif