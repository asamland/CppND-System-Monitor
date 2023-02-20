#ifndef PROCESS_H
#define PROCESS_H

#include <map>
#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid, std::map<int, std::string>&);
  int Pid() const;                         // TODO: See src/process.cpp
  std::string User() const;                // TODO: See src/process.cpp
  std::string Command() const;             // TODO: See src/process.cpp
  float CpuUtilization() const;            // TODO: See src/process.cpp
  std::string Ram() const;                 // TODO: See src/process.cpp
  long int UpTime() const;                 // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

  // TODO: Declare any necessary private members
 private:
  int pid_;
  int uid_;
  long int start_time_s_;
  std::string user_;
  std::string cmd_;
};

#endif