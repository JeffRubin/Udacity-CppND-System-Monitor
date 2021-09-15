#include <algorithm>
#include <unistd.h>
#include <cctype>
#include <cmath>
#include <sstream>
#include <string>
#include <vector>

#include "config.h"
#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

using LinuxParser::ProcessTimes;

long Process::ticks_to_seconds_ = sysconf(_SC_CLK_TCK);

Process::Process(int pid): pid_(pid) {}

void Process::Update() {
    command_ = LinuxParser::Command(pid_);
    user_ = LinuxParser::User(LinuxParser::Uid(pid_));
    CalculateCpuUtlization();
    ram_ = to_string(static_cast<unsigned long long>(round(std::stoull(LinuxParser::Ram(pid_))/(KB_IN_MB))));
}

// This function must be called twice, once and then again after a delay of PROCESSOR_UTILIZATION_DELAY in order for the calculation of CPU utilization to complete
void Process::CalculateCpuUtlization() {
    vector<unsigned long long> process_times = LinuxParser::ProcessorUtilization(pid_);
    // Maybe add support in the future for Linux versions before version 2.6 where uptime was in jiffies instead of ticks, but Linux 2.6 itself is fairly old and not actively supported: https://www.linuxfoundation.org/blog/the-2-6-32-linux-kernel/
    uptime_ = round(static_cast<float>(process_times[ProcessTimes::kStartTime_])/ticks_to_seconds_);
    
    // algorithm based on: https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
    unsigned long long cur_total_process_runtime = process_times[ProcessTimes::kUTime_] + process_times[ProcessTimes::kSTime_] + process_times[ProcessTimes::kCUTime_] + process_times[ProcessTimes::kCSTime_];
    if(prev_total_process_runtime != 0) { // calculate on second call of this function since need data from the first call for the calculation to be valid
        cpu_utilization_ = (static_cast<float>(cur_total_process_runtime - prev_total_process_runtime)/ticks_to_seconds_)/(PROCESSOR_UTILIZATION_DELAY/MS_IN_SEC);
        cpu_utilization_ = std::min(cpu_utilization_, 1.0f); // cap transients
    }
    else {
        cpu_utilization_ = 0.0;
    }
    
    prev_total_process_runtime = cur_total_process_runtime;
}
