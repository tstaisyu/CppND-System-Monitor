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
          break;
        }
      }
      while (linestream >> Title >> Value) {
        if (Title == "MemFree") {
          Free = Value;
          break;
        }
      }
    }
  }
  return stoi(Free) / stoi(Total);
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

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  return LinuxParser::UpTime() * sysconf(_SC_CLK_TCK);
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  long value;
  for (auto v : LinuxParser::CpuUtilization() ) {
    value += stol(v);
  }
  return value;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  return stol(LinuxParser::CpuUtilization()[3]) + stol(LinuxParser::CpuUtilization()[4]);
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string line;
  string Title, v0, v1 ,v2, v3, v4, v5, v6, v7, v8, v9;
  string Total, Free;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> Title >> v0 >> v1 >> v2 >> v3 >> v4 >> v5 >> v6 >> v7 >> v8 >> v9) {
        if (Title == "cpu") {
          break;
        }
      }
    }
  }
  return {v0, v1 ,v2, v3, v4, v5, v6, v7, v8, v9};
}  


// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line;
  string Title, Value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> Title >> Value) {
        if (Title == "processes") {
          break;
        }
      }
    }
  }
  return stoi(Value);
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line;
  string Title, Value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> Title >> Value) {
        if (Title == "procs_running") {
          break;
        }
      }
    }
  }
  return stoi(Value);
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }
