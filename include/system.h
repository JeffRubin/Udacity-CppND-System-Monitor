#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "updateable.h"

class System : public Updateable {
 public:
  System();
  void Update() override;
  Processor& Cpu();
  std::vector<Process>& Processes();
  float MemoryUtilization();
  long UpTime();
  int TotalProcesses();
  int RunningProcesses();
  std::string Kernel() const;
  std::string OperatingSystem() const;
  void SetSortBy(char s) { if(s == 'c' || s == 'm' || s == 'i') {sort_by_ = s;} } //cpu utilization (c) or memory (m) or pid ascending (i) [i is mostly for testing]

 private:
  Processor cpu_ = {};
  std::vector<Process> processes_ = {};
  float memory_utilization_{0.0};
  long up_time_{0};
  int total_processes_{0};
  int running_proceses_{0};
  const std::string kernel_{"null kernel"};
  const std::string operating_system_{"null os"};
  char sort_by_{'m'};
};

#endif