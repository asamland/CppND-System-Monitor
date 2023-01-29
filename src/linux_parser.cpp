#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <string>
#include <vector>
#include <map>

using std::stof;
using std::string;
using std::to_string;
using std::vector;
using std::map;
using std::pair;

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
  string os, version, kernel;
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

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line, key, value, mem_total_key, mem_free_key;
  long int mem_total = 0, mem_free = 0;
  mem_total_key = "MemTotal:";
  mem_free_key = "MemFree:";

  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == mem_total_key) {
        mem_total = std::stol(value);
      } else if (key == mem_free_key) {
        mem_free = std::stol(value);
      }
      if ((mem_free > 0) && (mem_total > 0)) {
        return (mem_total - mem_free) / (float)(mem_total);
      }
    }
  }
  return 0;
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() {
  string system_uptime_s, system_idle_time_s;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> system_uptime_s >> system_idle_time_s;
  }
  return std::stol(system_uptime_s);
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  string line, key, user, nice, system, idle, iowait, irq, softirq, steal,
      guest, guest_nice, cpu_key;
  cpu_key = "cpu";
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == cpu_key) {
        linestream >> user >> nice >> system >> idle >> iowait >> irq >>
            softirq >> steal >> guest >> guest_nice;
        return std::stol(user) + std::stol(nice) + std::stol(system) + std::stol(idle) + std::stol(iowait) + std::stol(irq) + std::stol(softirq) + std::stol(steal) + std::stol(guest) + std::stol(guest_nice);
      }
    }
  }
  return 0;
}

// DONE: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  string line, value;
  int utime_index = 14, stime_index = 15, cutime_index = 16, cstime_index = 17, active_jiffies = 0;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      for(int i = 1; i <=cstime_index; i++)
      {
        linestream >> value;
        if (i == utime_index || i == stime_index || i == cutime_index || i == cstime_index) {
          active_jiffies += std::stoi(value);
        }
      }
      return active_jiffies;
    }
  }
  return 0;
}

// DONE: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  string line, key, user, nice, system, idle, iowait, irq, softirq, steal,
      guest, guest_nice, cpu_key;
  cpu_key = "cpu";
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == cpu_key) {
        linestream >> user >> nice >> system >> idle >> iowait >> irq >>
            softirq >> steal >> guest >> guest_nice;
        return std::stol(user) + std::stol(nice) + std::stol(system) + std::stol(irq) + std::stol(softirq) + std::stol(steal) + std::stol(guest) + std::stol(guest_nice);
      }
    }
  }
  return 0;
}

// DONE: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  string line, key, user, nice, system, idle, iowait, irq, softirq, steal,
      guest, guest_nice, cpu_key;
  cpu_key = "cpu";
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == cpu_key) {
        linestream >> user >> nice >> system >> idle >> iowait >> irq >>
            softirq >> steal >> guest >> guest_nice;
        return std::stol(idle) + std::stol(iowait);
      }
    }
  }
  return 0;
}

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string line, key, user, nice, system, idle, iowait, irq, softirq, steal,
      guest, guest_nice, cpu_key;
  cpu_key = "cpu";
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == cpu_key) {
        linestream >> user >> nice >> system >> idle >> iowait >> irq >>
            softirq >> steal >> guest >> guest_nice;
        return vector<string>{user, nice,    system, idle,  iowait,
                              irq,  softirq, steal,  guest, guest_nice};
      }
    }
  }
  return vector<string>{};
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line, key, value, process_key;
  process_key = "processes";
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == process_key) {
        return std::stoi(value);
      }
    }
  }
  return 0;
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line, key, value, process_key;
  process_key = "procs_running";
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == process_key) {
        return std::stoi(value);
      }
    }
  }
  return 0;
}

// DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string line;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    return line;
  }
  return string();
}

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string line, key, value, memory_key, unit;
  memory_key = "VmSize:";
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value >> unit;
      if (key == memory_key) {
        if (unit == "kB")
        {
          return std::to_string(std::stoi(value)/1024);
        }
        else
        {
          return value;
        }
      }
    }
  }return string(); }

// DONE: Read and return the user ID associated with a process
int LinuxParser::Uid(int pid) {
  string line, key, value, uid_key;
  uid_key = "Uid:";
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == uid_key) {
        return std::stoi(value);
      }
    }
  }
  return -1;

}

map<int, string> LinuxParser::UidToUserMap() {
  map<int, string> uid_to_user_map;
  string line, user, x, uid;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      // x is a throw away value here
      linestream >> user >> x >> uid;
      uid_to_user_map.insert(pair<int, string>(std::stoi(uid), user));
    }
  }
  return uid_to_user_map;

}
// DECIDED ON ALTERNATE IMPLEMENTATION
// // TODO: Read and return the user associated with a process
// // REMOVE: [[maybe_unused]] once you define the function
// string LinuxParser::User(int pid) {}

// DONE: Read and return the start time of a process
long LinuxParser::ProcStartTime(int pid) {
  string line, value;
  int start_time_index = 22;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      for(int i = 1; i <=start_time_index; i++)
      {
        linestream >> value;
        if (i == start_time_index) {
          return std::stoi(value);
        }
      }
    }
  }
  return 0;
}
