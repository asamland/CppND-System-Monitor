#include <unistd.h>
#include <vector>

#include "linux_parser.h"
#include "processor.h"

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() {
  const static long jiffies_per_s = sysconf(_SC_CLK_TCK);
  static long prev_jiffies = 0;
  static long int prev_active_jiffies = 0;
  long curr_jiffies = LinuxParser::Jiffies();
  long curr_active_jiffies = LinuxParser::ActiveJiffies();
  long delta_jiffies = curr_jiffies - prev_jiffies;
  // throttle calculation to 1 Hz
  if (delta_jiffies > jiffies_per_s) {
    long int delta_active_jiffies = curr_active_jiffies - prev_active_jiffies;
    cpu_utilization_ = (float)delta_active_jiffies / delta_jiffies;
    prev_active_jiffies = curr_active_jiffies;
    prev_jiffies = curr_jiffies;
  }
  return cpu_utilization_;
}