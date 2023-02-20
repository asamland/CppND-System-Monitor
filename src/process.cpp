#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid, std::map<int, std::string>& uid_to_user_map)
    : pid_{pid}, uid_{LinuxParser::Uid(pid)}, cmd_{LinuxParser::Command(pid)} {
  // start time in jiffies divided by system clock tick rate (Hz)
  start_time_s_ = LinuxParser::ProcStartTime(pid_) / sysconf(_SC_CLK_TCK);
  auto pair = uid_to_user_map.find(uid_);
  if (pair != uid_to_user_map.end()) {
    user_ = pair->second;
  } else {
    user_ = "unknown_user";
  }
}

// DONE: Return this process's ID
int Process::Pid() const { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
  static long prev_up_time_s = 0;
  static long int prev_active_time_s = 0;
  long curr_up_time_s = UpTime();
  long delta_up_time_s = curr_up_time_s - prev_up_time_s;
  // throttle calculation to 1 Hz
  if (delta_up_time_s > 1) {
    long int curr_active_time_s =
        LinuxParser::ActiveJiffies(pid_) / sysconf(_SC_CLK_TCK);
    long int delta_active_time_s = curr_active_time_s - prev_active_time_s;
    cpu_utilization_ = (float)delta_active_time_s / delta_up_time_s;
    prev_active_time_s = curr_active_time_s;
    prev_up_time_s = curr_up_time_s;
  }
  return cpu_utilization_;
}

// DONE: Return the command that generated this process
string Process::Command() const { return cmd_; }

// DONE: Return this process's memory utilization
string Process::Ram() const { return LinuxParser::Ram(pid_); }

// DONE: Return the user (name) that generated this process
string Process::User() const { return user_; }

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() const {
  // return system uptime minus process start time
  return LinuxParser::UpTime() - start_time_s_;
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator>(Process const& a) const {
  return this->cpu_utilization_ > a.cpu_utilization_;
}