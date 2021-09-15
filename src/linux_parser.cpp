#include <dirent.h>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

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
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
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

vector<unsigned long long> LinuxParser::MemoryUtilization() {
  vector<unsigned long long> memory;

  string label, value, units;
  string line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    std::istringstream linestream;

    label = "";
    while(stream && (label != "MemTotal")) { // seems more robust than assuming it's the first row (e.g. if the file format later changes), also need a way to end if never find it
      std::getline(stream, line);
      linestream.str(line);
      linestream.clear(); // https://stackoverflow.com/questions/2767298/c-repeatedly-using-istringstream
      linestream >> label >> value >> units;
      label.erase(remove(label.begin(), label.end(), ':'), label.end());
      memory.push_back(std::stoull(value));
    }
    
    label = "";
    while(stream && (label != "MemFree")) {
      std::getline(stream, line);
      linestream.str(line);
      linestream.clear();
      linestream >> label >> value >> units;
      label.erase(remove(label.begin(), label.end(), ':'), label.end());
      memory.push_back(std::stoull(value));
    }
  }
  else {
    for(int i=0; i<2; i++) {
      memory.push_back(0);
    }
  }

  return memory;
}

unsigned long long LinuxParser::UpTime() {
  string uptime, idle_time;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idle_time;
    return static_cast<unsigned long long>(stof(uptime));      
  }
  else {
    return 0;
  }
}

vector<unsigned long long> LinuxParser::CpuUtilization() {
  vector<unsigned long long> cpu_utilization;

  string line, part;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::istringstream linestream;

    string label = "";
    while(stream && (label != "cpu")) {
      std::getline(stream, line);
      linestream.str(line);
      linestream >> label;
      if(label != "cpu") {
        continue;
      }
      else {
        for(int i=0; i<10; i++) {
          linestream >> part;
          cpu_utilization.push_back(stoull(part));
        }
      }
    }
  }
  else {
    for(int i=0; i<10; i++) {
      cpu_utilization.push_back(0);
    }
  }

  return cpu_utilization;
}

vector<int> LinuxParser::Processes() {
  vector<int> process_counts;
  string label, value;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::istringstream linestream;

    label = "";
    while(stream && (label != "processes")) {
      std::getline(stream, line);
      linestream.str(line);
      linestream.clear();
      linestream >> label >> value;
    }
    process_counts.push_back(stoi(value));
    
    label = "";
    while(stream && (label != "procs_running")) {
      std::getline(stream, line);
      linestream.str(line);
      linestream.clear();
      linestream >> label >> value;
    }
    process_counts.push_back(stoi(value));
  }
  else {
    for(int i=0; i<2; i++) {
      process_counts.push_back(0);
    }
  }

  return process_counts;
}

string LinuxParser::Command(int pid) {
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::istringstream linestream;
    std::getline(stream, line);
    line.erase(remove(line.begin(), line.end(), '\000'), line.end());
    return line;
  }
  else {
    return "null";
  }
}

string LinuxParser::Ram(int pid) {
  string label, value;
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    std::istringstream linestream;

    label = "";
    while(stream && (label != "VmSize")) {
      std::getline(stream, line);
      linestream.str(line);
      linestream.clear();
      linestream >> label >> value;
      label.erase(remove(label.begin(), label.end(), ':'), label.end());
    }
    return value;
  }
  else {
    return "0.0";
  }
}

string LinuxParser::Uid(int pid) {
  string line, value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    std::istringstream linestream;
    string label = "";
    while(stream && (label != "Uid")) {
      std::getline(stream, line);
      linestream.str(line);
      linestream.clear();
      linestream >> label >> value;
      label.erase(remove(label.begin(), label.end(), ':'), label.end());
    }
    return value;      
  }
  else {
    return "null";
  }
}

string LinuxParser::User(std::string uid_in) {
  string line, x, user;
  std::ifstream stream(kPasswordPath);
  if (uid_in != "null" && stream.is_open()) {
    std::istringstream linestream;
    string uid = "";
    while(stream && (uid != uid_in)) {
      std::getline(stream, line);
      std::replace(line.begin(), line.end(), ':', ' ');
      linestream.str(line);
      linestream.clear();
      linestream >> user >> x >> uid;
    }
    return user;      
  }
  else {
    return "null";
  }
}

vector<unsigned long long> LinuxParser::ProcessorUtilization(int pid) {
  vector<unsigned long long> times;
  string line, value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::istringstream linestream;
    std::getline(stream, line);
    linestream.str(line);

    for(int v=1; v<=22; v++) {
      linestream >> value;
      linestream.clear();

      if((v>=14 && v<=17) || (v==22)) {
        times.push_back(std::stoull(value));
      }
    }
  }
  else {
    for(int i=0; i<5; i++) {
      times.push_back(0);
    }
  }
  return times;
}
