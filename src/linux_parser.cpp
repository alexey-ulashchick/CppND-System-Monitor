#include "linux_parser.h"
#include <dirent.h>
#include <unistd.h>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

long long LinuxParser::ProcessState::TotalTime() {
  return utime + stime + cutime + cstime;
}

string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

LinuxParser::ProcessorState LinuxParser::CpuUtilization() {
  std::ifstream stream(kProcDirectory + kStatFilename);
  std::string line, label;
  long user, nice, system, idle, iowait, irq, softirq, steal;
  long cNonIdle, cIdle;

  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> label >> user >> nice >> system >> idle >> iowait >> irq >>
        softirq >> steal;
    cNonIdle = user + nice + system + irq + softirq + steal;
    cIdle = idle + iowait;

    return LinuxParser::ProcessorState{cNonIdle, cIdle};
  }

  return LinuxParser::ProcessorState{0, 0};
}

/**
 * Returns total memory utilization including caches and buffers.
 * */
float LinuxParser::MemoryUtilization() {
  float total, free;
  string label, line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);  // This is TOTAL memeory.
    linestream >> label >> total;

    std::getline(stream, line);  // This is AVAILABLE memory.
    linestream.clear();
    linestream.str(line);
    linestream >> label >> free;
  }

  return (total - free) / total;
}

int LinuxParser::TotalProcesses() { return LinuxParser::Pids().size(); }

int LinuxParser::RunningProcesses() {
  std::ifstream stream(kProcDirectory + kStatFilename);
  std::string line, label;
  int procsRunning;

  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> label;
      if (label != "procs_running") continue;
      linestream >> procsRunning;

      return procsRunning;
    }
  }

  return 0;
}

long long LinuxParser::UpTime() {
  float upTime = 0;
  std::string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);

  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> upTime;
  }
  return static_cast<long long>(upTime);
}

std::unordered_map<int, std::string> LinuxParser::GetAllUsers() {
  std::unordered_map<int, std::string> users{};
  std::ifstream stream(kPasswordPath);
  std::string line, name, _tmp;
  int id;

  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> name >> _tmp >> id;
      users.insert({id, name});
    }
  }

  return users;
}

vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    if (file->d_type == DT_DIR) {
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

int LinuxParser::Uid(int pid) {
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  std::string line, key;
  int uid;

  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key != "Uid:") continue;
      linestream >> uid;

      return uid;
    }
  }

  return uid;
}

LinuxParser::ProcessState LinuxParser::GetProcessState(int pid) {
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  std::string line, v;
  LinuxParser::ProcessState ps;

  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);

    // Skipping first 1-13 values;
    for (auto i = 1; i <= 13; i++) {
      linestream >> v;
    }

    linestream >> ps.utime >> ps.stime >> ps.cutime >> ps.cstime;

    // Skipping first 18-21 values;
    for (auto i = 18; i <= 21; i++) {
      linestream >> v;
    }

    linestream >> ps.starttime;
  }

  return ps;
}

string LinuxParser::Command(int pid) {
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  std::string line;

  if (stream.is_open()) {
    std::getline(stream, line);
    return line;
  }

  return string();
}

string LinuxParser::Ram(int pid) {
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  std::string line, key;
  long kb;

  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key != "VmSize:") continue;
      linestream >> kb;

      std::stringstream ss;
      ss << std::fixed << std::setprecision(1) << kb / 1024.0f;

      return ss.str();
    }
  }

  return string();
}