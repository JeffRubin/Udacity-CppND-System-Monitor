#ifndef PROCESS_H
#define PROCESS_H

#include "updateable.h"

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process : public Updateable {
 public:
  Process(int pid);
  void Update() override;
  int Pid() const { return pid_; }
  std::string User() const { return user_; }
  std::string Command() const { return command_; }
  static long ticks_to_seconds_;
  void CalculateCpuUtlization();
  float CpuUtilization() const { return cpu_utilization_; }
  std::string Ram() const { return ram_; }
  unsigned long long UpTime() const { return uptime_; }

 private:
  int pid_{0};
  std::string user_{"null"};
  std::string command_{"null"};

  unsigned long long prev_total_process_runtime{0};
  float cpu_utilization_{0.0};
  
  std::string ram_{"null"};
  unsigned long long uptime_{0};
};

#endif
