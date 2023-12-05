#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  // constructor
  Process(int pid);
  int Pid() const;                               // TODO: See src/process.cpp
  std::string User() const;                      // TODO: See src/process.cpp
  std::string Command() const;                   // TODO: See src/process.cpp
  float CpuUtilization() const;                  // TODO: See src/process.cpp
  std::string Ram() const;                       // TODO: See src/process.cpp
  long int UpTime() const;                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

  // TODO: Declare any necessary private members
 private:
   enum ProcessStates {
     Utime_ = 0,
     Stime_,
     Cutime_,
     Cstime_,
     Starttime_
   };
   int m_pid;
   std::string m_user;
   std::string m_command;
   float m_processUsage;
   std::string m_ram;
   long int m_upTime; 
   float CalProcessUsage();
};

#endif