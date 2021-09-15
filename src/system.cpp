#include <unistd.h>
#include <algorithm>
#include <chrono>
#include <cstddef>
#include <set>
#include <string>
#include <thread>
#include <vector>

#include "config.h"
#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

using LinuxParser::ProcessCounts;
using LinuxParser::MemoryParts;

bool SortByCpuUtil(const Process & a, const Process & b) { return a.CpuUtilization() > b.CpuUtilization(); }
bool SortByMemUtil(const Process & a, const Process & b) { return std::stoull(a.Ram()) > std::stoull(b.Ram()); }
bool SortByPid(const Process & a, const Process & b) { return a.Pid() > b.Pid(); }

System::System(): kernel_(LinuxParser::Kernel()), operating_system_(LinuxParser::OperatingSystem()) {}

void System::Update() {
    // Processor
    cpu_.Update();

    // Processes
    processes_.clear();
    vector<int> pids = LinuxParser::Pids();

    for(int pid : pids) {
        processes_.push_back(Process(pid));
    }   
    for(Process & process : processes_) {
        process.Update(); // includes first call of CalculateCpuUtlization()
    }
    
    std::this_thread::sleep_for(std::chrono::milliseconds(PROCESSOR_UTILIZATION_DELAY));
    
    for(Process & process : processes_) {
        process.CalculateCpuUtlization(); // second call of CalculateCpuUtlization()
    }

    bool (*sort_func)(const Process &, const Process & );
    switch(sort_by_) {
        case 'c':
            sort_func = SortByCpuUtil;
            break;
        case 'm':
            sort_func = SortByMemUtil;
            break;
        case 'i':
            sort_func = SortByPid;
            break;
        default: // shouldn't get here but do something if somehow end up here
            sort_func = SortByMemUtil;
            break;
    }


    std::sort(processes_.begin(), processes_.end(), sort_func);

    // System
    vector<unsigned long long> memory = LinuxParser::MemoryUtilization();
    unsigned long long mem_total = memory[MemoryParts::kMemTotal_];
    if(mem_total != 0) {
        memory_utilization_ = static_cast<float>(mem_total - memory[MemoryParts::kMemFree_])/mem_total;
        memory_utilization_ = std::min(memory_utilization_, 1.0f); // cap transients
    }
    else {
        memory_utilization_ = 0.0;
    }

    vector<int> process_counts = LinuxParser::Processes();
    total_processes_ = process_counts[ProcessCounts::kTotalProcesses_];
    running_proceses_ = process_counts[ProcessCounts::kRunningProcesses_];

    up_time_ = LinuxParser::UpTime();
};

Processor& System::Cpu() { return cpu_; }

vector<Process>& System::Processes() { return processes_; }

std::string System::Kernel() const { return kernel_; }

float System::MemoryUtilization() { return memory_utilization_; }

std::string System::OperatingSystem() const { return operating_system_; }

int System::RunningProcesses() { return running_proceses_; }

int System::TotalProcesses() { return total_processes_; }

long int System::UpTime() { return up_time_; }