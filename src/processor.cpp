#include <algorithm>

#include "linux_parser.h"
#include "processor.h"

using LinuxParser::CPUStates;

void Processor::Update() {
std::vector<unsigned long long> cpu = LinuxParser::CpuUtilization();

// algorithm based on: https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
unsigned long long cur_idle = cpu[CPUStates::kIdle_] + cpu[CPUStates::kIOwait_];
unsigned long long cur_nonidle = cpu[CPUStates::kUser_] + cpu[CPUStates::kNice_] + cpu[CPUStates::kSystem_] + cpu[CPUStates::kIRQ_] + cpu[CPUStates::kSoftIRQ_] + cpu[CPUStates::kSteal_];
unsigned long long cur_total = cur_idle + cur_nonidle;

unsigned long long total_diff = cur_total - prev_total_;
unsigned long long nonidle_diff = cur_nonidle - prev_nonidle_;

if((total_diff != 0) && (prev_total_ != 0)) { // protect against divide by 0 and skip on first cycle when prev_total hasn't been calculated yet
    utilization_ = static_cast<float>(nonidle_diff)/total_diff;
    utilization_ = std::min(utilization_, 1.0f); // cap transients
}
else {
    utilization_ = 0.0;
}

 // update previous values
prev_nonidle_ = cur_nonidle;
prev_total_ = cur_total;

}
