#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>
#include <map>

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
float MemoryUtilization();
long long UpTime();
std::vector<int> Pids();
int TotalProcesses();
int RunningProcesses();
std::string OperatingSystem();
std::string Kernel();
std::map<int, std::string> GetAllUsers();

// CPU
// enum CPUStates {
//   kUser_ = 0,
//   kNice_,
//   kSystem_,
//   kIdle_,
//   kIOwait_,
//   kIRQ_,
//   kSoftIRQ_,
//   kSteal_,
//   kGuest_,
//   kGuestNice_
// };
ProcessorState CpuUtilization();


long Jiffies();
long ActiveJiffies();
long ActiveJiffies(int pid);
long IdleJiffies();

// Processes
int Uid(int pid);
ProcessState GetProcessState(int pid);


std::string Command(int pid);
std::string Ram(int pid);

std::string User(int pid);
long int UpTime(int pid);
};  // namespace LinuxParser

#endif