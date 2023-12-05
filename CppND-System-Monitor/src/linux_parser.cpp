#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

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
  string os, kernel, version;
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string memKey;
  string memVal;
  float memTotal = 1;
  float memFree = 1;
  string line;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
//       std::remove(line.begin(), line.end(), ' ');
//       std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> memKey >> memVal;
      if (memKey == "MemTotal:") {
          memTotal = stof(memVal);
      } else if (memKey == "MemAvailable:") {
        memFree = stof(memVal);
        float memUtilization = (memTotal - memFree) / memTotal;
        return memUtilization;
      }
    }
  }
  return 0.0;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  string upTime;
  string line;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> upTime;
  }
  return long(stol(upTime));
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  auto jiffies = CpuUtilization();
  try {
    long sum = 0;
    for (unsigned int i = 0; i < (jiffies.size() - 2); ++i) {
      sum += stol(jiffies[i]); 
    }
    return sum;
  } catch(...) {
    std::cout << "invalid stol argument or stol argument out of range\n";
    return 0;
  }
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  long activeJiffies = LinuxParser::Jiffies() - LinuxParser::IdleJiffies();
  return activeJiffies;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  auto jiffies = CpuUtilization();
  try {
    long sum = stol(jiffies[CPUStates::kIdle_]) + stol(jiffies[CPUStates::kIOwait_]);
    return sum;
} catch(...) {
    std::cout << "invalid stol argument or stol argument out of range\n";
    return 0;
  }
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  // I believe this part is not very efficient
  vector<string> result;
  string cpuKey, kUser, kNice, kSystem, kIdle, kIOwait, kIRQ, kSoftIRQ, 
    kSteal, kGuest, kGuestNice;
  string line;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {     
      std::istringstream linestream(line);
      while (linestream >> cpuKey >> kUser >> kNice >> kSystem >> kIdle >> kIOwait >> kIRQ >> kSoftIRQ
        >> kSteal >> kGuest >> kGuestNice) {
        if (cpuKey == "cpu") {
          result.emplace_back(kUser);
          result.emplace_back(kNice);
          result.emplace_back(kSystem);
          result.emplace_back(kIdle);
          result.emplace_back(kIOwait);
          result.emplace_back(kIRQ);
          result.emplace_back(kSoftIRQ);
          result.emplace_back(kSteal);
          result.emplace_back(kGuest);
          result.emplace_back(kGuestNice);
          return result;
        }
      }
    }
  }
  return {};   
}
// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string processesKey;
  string processesValue;
  string line;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {     
      std::istringstream linestream(line);
      while (linestream >> processesKey >> processesValue) {   
        if (processesKey == "processes")  {
          try {
            return stoi(processesValue);
          } catch (...) {
            std::cout << "invalid stol argument or stol argument out of range\n";
            return 0;
          }
        }   
      }
    }
  }
  return 0;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string processesKey;
  string processesValue;
  string line;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {     
      std::istringstream linestream(line);
      while (linestream >> processesKey >> processesValue) {   
        if (processesKey == "procs_running")  {
          try {
            return stoi(processesValue);
          } catch (...) {
            std::cout << "invalid stol argument or stol argument out of range\n";
            return 0;
          }
        }   
      }
    }
  }
  return 0;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string line = "";
  std::ifstream filestream(kProcDirectory +std::to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    return line;
  }
  return line;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string ramKey;
  string ramStr("");
  string line;
  std::ifstream filestream(kProcDirectory +std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while(std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> ramKey;
      if (ramKey == "VmSize:") {
        linestream >> ramStr;
        break;
      }
    }  
  }
  return ramStr;
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string ramKey;
  string ramStr("");
  string line;
  std::ifstream filestream(kProcDirectory +std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while(std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> ramKey;
      if (ramKey == "Uid:") {
        linestream >> ramStr;
        break;
      }
    }  
  }
  return ramStr;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string uid = Uid(pid);
  string id, x, temp;
  string line;
  string name = "DEFAULT";
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while(std::getline(filestream, line)) {
       std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> temp >> x >> id;
      if (id == uid) {
        name = temp;
        break;
      }
    }  
  }
  return name;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  string line, val;
  long upTime = 0;
  string value;
  std::ifstream filestream(kProcDirectory +std::to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for (unsigned int i = 0; i < ProcessStates::kStartTime_; ++i) {
      linestream >> val; 
    }
    linestream >> val;
    try {
      upTime = LinuxParser::UpTime() - (stol(val) / sysconf(_SC_CLK_TCK));
    } catch (...) {
      std::cout << "invalid stol argument or stol argument out of range\n";
      upTime = 0;
    }    
  }
  return upTime;
}

vector<float> LinuxParser::ProcessUtilization(int pid) {
  vector<float> processVal{};
  string val;
  float time = 0;
  string line;
  std::ifstream filestream(kProcDirectory +std::to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for (unsigned int i = 0; i <= ProcessStates::kStartTime_; ++i) {
      linestream >> val;
      if (i == ProcessStates::kUtime_ ||  i == ProcessStates::kStime_ ||
          i == ProcessStates::kCstime_ ||
          i == ProcessStates::kCutime_ ||  i == ProcessStates::kStartTime_) {
        time = std::stof(val) / sysconf(_SC_CLK_TCK);
        processVal.push_back(time);
      } 
    }   
  }
  return processVal;
}
