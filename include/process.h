#ifndef PROCESS_H
#define PROCESS_H

#include <string>

/**
 * Basic class for Process representation.
 * It contains relevant attributes as shown below.
 */
class Process {
 public:
  Process() = default;
  /**
   * Constucts process object. Stores PID, where:
   *    pid           - process ID,
   *    user          - process owner name,
   *    command       - command name,
   *    ram           - amount of RAM used by process,
   *    startTime     - process start time,
   *    hz            - number of clock ticks per second configured in Kernel,
   *    totalTime     - total time in processor ticks that this process used,
   *    upTime        - total time since syste was started.
   */
  Process(int pid, std::string user, std::string command, std::string ram,
          long long startTime, float hz, long long totalTime, long long upTime)
      : pid{pid},
        user{user},
        command{command},
        ram{ram},
        startTime{startTime},
        hz{hz},
        prevTotalTime{totalTime},
        prveUpTime{upTime} {};

  /**
   * Updates state of the process and recalculates utilization.
   *    nextTotalTime - total time in processor ticks that this process used,
   *    upTime        - total time in processor ticks since it was started.
   */
  void Update(const long long& nextTotalTime, const long long& nextUpTime,
              const long long& startTime);

  const int& GetPid();
  const float& GetCpuUtilization();
  long long GetUpTime();
  const std::string& GetUser();
  const std::string& GetCommand();
  const std::string& GetRam();

  bool operator<(Process const& a) const;

 private:
  int pid;
  std::string user;
  std::string command;
  std::string ram;
  long long startTime;
  float hz;
  long long prevTotalTime;
  long long prveUpTime;

  float utilization = 0;
};

#endif