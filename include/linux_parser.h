#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

namespace LinuxParser {
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
enum ProcessCounts {
  kTotalProcesses_ = 0,
  kRunningProcesses_
};
enum MemoryParts {
  kMemTotal_ = 0,
  kMemFree_
};
std::vector<unsigned long long> MemoryUtilization();
unsigned long long UpTime();
std::vector<int> Pids();
std::vector<int> Processes();
std::string OperatingSystem();
std::string Kernel();

// CPU
enum CPUStates {
  kUser_ = 0,
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_,
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_
};
std::vector<unsigned long long> CpuUtilization();

// Processes
enum ProcessTimes {
  kUTime_ = 0,
  kSTime_,
  kCUTime_,
  kCSTime_,
  kStartTime_
};
std::string Command(int pid);
std::string Ram(int pid);
std::string Uid(int pid);
std::string User(std::string uid);
std::vector<unsigned long long> ProcessorUtilization(int pid);
};  // namespace LinuxParser

#endif