#include "processor.h"

#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  // Source: https://knowledge.udacity.com/questions/869207
  float active, total;
  active = LinuxParser::ActiveJiffies();
  total = LinuxParser::Jiffies();
  return active / total;
}