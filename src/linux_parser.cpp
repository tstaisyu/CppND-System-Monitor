#include <dirent.h>
#include <unistd.h>

#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
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

// DONE: An example of how to read data from the filesystem
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

// BONUS: Update this to use std::filesystem
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  // Source:
  // https://stackoverflow.com/questions/41224738/how-to-calculate-system-memory-usage-from-proc-meminfo-like-htop/41251290#41251290

  string line;
  string Title, Value;
  string Total, Free;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> Title >> Value) {
        if (Title == "MemTotal") {
          Total = Value;
          continue;
        } else if (Title == "MemFree") {
          Free = Value;
          continue;
        }
      }
    }
  }
  return (stof(Total) - stof(Free)) / stof(Total);
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  string Value;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> Value;
  }
  return stol(Value);
}

// Source: https://knowledge.udacity.com/questions/129844

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  return LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies();
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  string v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15,
      v16;
  string line;
  long total;
  std::ifstream stream(kProcDirectory + "/" + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::replace(line.begin(), line.end(), '(', ' ');
    std::replace(line.begin(), line.end(), ')', ' ');
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> v0 >> v1 >> v2 >> v3 >> v4 >> v5 >> v6 >> v7 >> v8 >> v9 >>
        v10 >> v11 >> v12 >> v13 >> v14 >> v15 >> v16;
  }
  total = stol(v13) + stol(v14) + stol(v15) + stol(v16);
  return total / sysconf(_SC_CLK_TCK);
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  return stol(LinuxParser::CpuUtilization()[CPUStates::kUser_]) + stol(LinuxParser::CpuUtilization()[CPUStates::kNice_]) + 
         stol(LinuxParser::CpuUtilization()[CPUStates::kSystem_]) + stol(LinuxParser::CpuUtilization()[CPUStates::kIRQ_]) + 
         stol(LinuxParser::CpuUtilization()[CPUStates::kSoftIRQ_]) + stol(LinuxParser::CpuUtilization()[CPUStates::kSteal_]);
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  return stol(LinuxParser::CpuUtilization()[CPUStates::kIdle_]) + stol(LinuxParser::CpuUtilization()[CPUStates::kIOwait_]);
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string line;
  string cpu, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9;
  vector<string> ReturnVector;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> cpu >> v0 >> v1 >> v2 >> v3 >> v4 >> v5 >> v6 >>
             v7 >> v8 >> v9) {
        if (cpu == "cpu") {
          ReturnVector = {v0, v1, v2, v3, v4, v5, v6, v7, v8, v9};
        }
      }
    }
  }
  return ReturnVector;
}


// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line;
  string Title, Value, ReturnValue;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> Title >> Value) {
        if (Title == "processes") {
          ReturnValue = Value;
        }
      }
    }
  }
  return stoi(ReturnValue);
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line;
  string Title, Value, ReturnValue;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> Title >> Value) {
        if (Title == "procs_running") {
          ReturnValue = Value;
        }
      }
    }
  }
  return stoi(ReturnValue);
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string line;
  std::ifstream stream(kProcDirectory + "/" + to_string(pid) +
                       kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
  }
  return line;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string line;
  string Key, Value, ReturnValue;
  std::ifstream stream(kProcDirectory + "/" + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> Key >> Value) {
        if (Key == "VmSize") {
            ReturnValue = to_string(stol(Value) / 1000);
        }
      }
    }
  }
  if (Value == "") {
    ReturnValue = "0.0";
  }
  return ReturnValue;
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string line;
  string Key, Value, ReturnValue;
  std::ifstream stream(kProcDirectory + "/" + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> Key >> Value) {
        if (Key == "Uid") {
          ReturnValue = Value;
        }
      }
    }
  }
  return ReturnValue;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string line;
  string Key, Value, x, user;
  string uid = Uid(pid);
  bool name{false};
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line) && !name) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> Key >> x >> Value) {
        if (Value == uid) {
          user = Key;
          name = true;
        }
      }
    }
  }
  return user;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  string Value, tmp;
  string line;
  std::ifstream stream(kProcDirectory + "/" + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int i = 0; i < 22; i++) {
      if (i == 21) {
        linestream >> Value;
      } else {
        linestream >> tmp;
      }
    }
  }
  return LinuxParser::UpTime() - stol(Value);
}
