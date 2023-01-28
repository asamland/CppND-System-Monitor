#include "processor.h"

#include <vector>

#include "linux_parser.h"

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() {
  long total, non_idle;
  std::vector<std::string> cpu_utilization_stats =
      LinuxParser::CpuUtilization();
  total = LinuxParser::Jiffies();
  non_idle = LinuxParser::ActiveJiffies();
  return (float)non_idle / total;
}