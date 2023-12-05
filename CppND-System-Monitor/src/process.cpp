#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include "process.h"
#include "linux_parser.h"
using std::string;
using std::to_string;
using std::vector;

// total_time = utime + stime + cutime + cstime;
// seconds = uptime - (starttime / Hertz), where Hertz may equal sysconf(_SC_CLK_TCK)
// usage = total_time / seconds
float Process::CalProcessUsage() {
    vector<float> val = LinuxParser::ProcessUtilization(m_pid);
    long upTime = LinuxParser::UpTime();
    if (val.size() == 5) {
      float totalTime = val[Utime_] + val[Stime_] + val[Cutime_] + val[Cstime_];
      float seconds = upTime - val[Starttime_];
      return float(totalTime / seconds);
    } else {
        return 0;
    }
}

Process::Process(int pid) : m_pid(pid) {
  m_user = LinuxParser::User(m_pid);
  m_command = LinuxParser::Command(m_pid);
  m_processUsage = CalProcessUsage();
  m_ram = LinuxParser::Ram(m_pid);
  m_upTime = LinuxParser::UpTime(m_pid); 
}
// TODO: Return this process's ID
int Process::Pid() const { return m_pid; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const { return m_processUsage; }

// TODO: Return the command that generated this process

string Process::Command() const { return m_command; }

// TODO: Return this process's memory utilization
string Process::Ram() const { return m_ram; }

// TODO: Return the user (name) that generated this process
string Process::User() const { return m_user; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() const { return m_upTime; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
  return CpuUtilization() < a.CpuUtilization();
}