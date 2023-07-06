
#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;
using namespace LinuxParser;

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization

// Source: https://knowledge.udacity.com/questions/772002
float Process::CpuUtilization() const { 
  float process_cpu;
  long p_time = LinuxParser::UpTime(pid_);
  long p_total = LinuxParser::ActiveJiffies(pid_);
  if (p_time != 0 ) {
    process_cpu = float(p_total) / float(p_time);
  } else {
    process_cpu = 0.0;
  }
  return process_cpu / 100;
}
/*
// Source: https://knowledge.udacity.com/questions/139746
  string v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18;
  string line;
  string Value;
  long total_time;
  float time_seconds;
  std::ifstream stream(kProcDirectory + "/" + to_string(pid_) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
      linestream >> v0 >> v1 >> v2 >> v3 >> v4 >> v5 >> v6 >> v7 >> v8 >> v9 >>
        v10 >> v11 >> v12 >> v13 >> v14 >> v15 >> v16 >> v17 >> v18;
        total_time = stol(v15) + stol(v16) + stol(v17) + stol(v18);
  }
  time_seconds = total_time / sysconf(_SC_CLK_TCK);
  return time_seconds / LinuxParser::UpTime(); 
}*/

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// Source: https://knowledge.udacity.com/questions/270873

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
  return a.CpuUtilization() < CpuUtilization();
}