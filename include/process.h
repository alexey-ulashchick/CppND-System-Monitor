#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process() = default;
  Process(int pid, std::string user, float hz, long long prevTotalTime,
          long long prveUpTime)
      : pid{pid},
        user{user},
        hz{hz},
        prevTotalTime{prevTotalTime},
        prveUpTime{prveUpTime} {};

  void Update(const long long& nextTotalTime, const long long& nextUpTime);

  int GetPid();
  std::string GetUser();
  float GetCpuUtilization();

  std::string Command();                   // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

 private:
  int pid;
  std::string user;
  float hz;
  long long prevTotalTime;
  long long prveUpTime;
  float utilization = 0;
};

#endif