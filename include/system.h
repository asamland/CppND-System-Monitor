#ifndef SYSTEM_H
#define SYSTEM_H

#include <map>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  System();
  Processor& Cpu();                   // DONE: See src/system.cpp
  std::vector<Process>& Processes();  // TODO: See src/system.cpp
  float MemoryUtilization();          // DONE: See src/system.cpp
  long UpTime();                      // DONE: See src/system.cpp
  int TotalProcesses();               // DONE: See src/system.cpp
  int RunningProcesses();             // DONE: See src/system.cpp
  std::string Kernel();               // DONE: See src/system.cpp
  std::string OperatingSystem();      // DONE: See src/system.cpp

  // TODO: Define any necessary private members
 private:
  std::string kernel_;
  std::string os_;
  Processor cpu_ = {};
  std::vector<Process> processes_ = {};
  std::map<int, std::string> uid_to_user_map_;
};

#endif