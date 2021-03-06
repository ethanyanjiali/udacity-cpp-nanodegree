#include <dirent.h>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>
#include <cassert>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem()
{
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open())
  {
    while (std::getline(filestream, line))
    {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value)
      {
        if (key == "PRETTY_NAME")
        {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel()
{
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids()
{
  vector<int> pids;
  DIR *directory = opendir(kProcDirectory.c_str());
  struct dirent *file;
  while ((file = readdir(directory)) != nullptr)
  {
    // Is this a directory?
    if (file->d_type == DT_DIR)
    {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit))
      {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization()
{
  string name, kilobytes;
  string line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open())
  {
    int total_mem, free_mem;
    for (int i = 0; i < 2; i++)
    {
      std::getline(stream, line);
      std::istringstream linestream(line);
      linestream >> name >> kilobytes;
      switch (i)
      {
      case 0:
        total_mem = stoi(kilobytes);
        break;
      case 1:
        free_mem = stoi(kilobytes);
        break;
      default:
        break;
      }
    }

    return (total_mem - free_mem) / (float)total_mem;
  }
  return 0.0;
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime()
{
  string line, uptime_str;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  std::getline(stream, line);
  std::istringstream linestream(line);
  linestream >> uptime_str;
  long uptime = stol(uptime_str);
  return uptime;
}

long LinuxParser::IdleTime()
{
  string line, uptime_str, idletime_str;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  std::getline(stream, line);
  std::istringstream linestream(line);
  linestream >> uptime_str >> idletime_str;
  long idletime = stol(idletime_str);
  return idletime;
}

// DONE: Read and return the number of jiffies for the system
long LinuxParser::Jiffies()
{
  return ActiveJiffies() + IdleJiffies();
}

// DONE: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid)
{
  auto states = LinuxParser::CpuUtilization(pid);
  long utime = stol(states[LinuxParser::ProcessCPUStates::kUTime_]);
  float stime = stof(states[LinuxParser::ProcessCPUStates::kSTime_]);
  return utime + stime;
}

// DONE: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies()
{
  return (UpTime() - IdleTime()) * sysconf(_SC_CLK_TCK);
}

// DONE: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies()
{
  return IdleTime() * sysconf(_SC_CLK_TCK);
}

int LinuxParser::TotalCPUs()
{
  string line;
  string filename = kProcDirectory + kCpuinfoFilename;
  std::ifstream stream(filename);
  while (stream.is_open() and !stream.eof())
  {
    std::getline(stream, line);
    std::replace(line.begin(), line.end(), ' ', '_');
    std::replace(line.begin(), line.end(), ':', ' ');
    std::istringstream linestream(line);
    string name, value;
    linestream >> name >> value;
    if (name.rfind("cpu cores", 0) == 0)
    {
      std::replace(value.begin(), value.end(), '_', ' ');
      return stoi(value);
    }
  }
  return 0;
};

string LinuxParser::CPUModelName()
{
  string line;
  string filename = kProcDirectory + kCpuinfoFilename;
  std::ifstream stream(filename);
  while (stream.is_open() and !stream.eof())
  {
    std::getline(stream, line);
    std::replace(line.begin(), line.end(), ' ', '_');
    std::replace(line.begin(), line.end(), ':', ' ');
    std::istringstream linestream(line);
    string name, value;
    linestream >> name >> value;
    if (name.rfind("model name", 0) == 0)
    {
      std::replace(value.begin(), value.end(), '_', ' ');
      return value;
    }
  }
  return string();
}

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization()
{
  string line;
  vector<string> states;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    string name;
    linestream >> name;
    for (string state; linestream >> state;)
    {
      states.push_back(state);
    }
  }
  return states;
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses()
{
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  while (stream.is_open() and !stream.eof())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    string name, value;
    linestream >> name >> value;
    if (name.compare("processes") == 0)
    {
      return stoi(value);
    }
  }
  return 0;
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses()
{
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  while (stream.is_open() and !stream.eof())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    string name, value;
    linestream >> name >> value;
    if (name.compare("procs_running") == 0)
    {
      return stoi(value);
    }
  }
  return 0;
}

// DONE: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid)
{
  string line;
  string filename = kProcDirectory + std::to_string(pid) + kCmdlineFilename;
  std::ifstream stream(filename);
  std::getline(stream, line);
  return line;
}

// DONE: Read and return the memory used by a process in KB
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid)
{
  string line;
  string filename = kProcDirectory + std::to_string(pid) + kStatusFilename;
  std::ifstream stream(filename);
  while (stream.is_open() and !stream.eof())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    string name, value;
    linestream >> name >> value;
    if (name.compare("VmSize:") == 0)
    {
      return value;
    }
  }
  return string();
}

vector<string> LinuxParser::CpuUtilization(int pid)
{
  string line;
  vector<string> states;
  string filename = kProcDirectory + std::to_string(pid) + kStatFilename;
  std::ifstream stream(filename);
  if (stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (string state; linestream >> state;)
    {
      states.push_back(state);
    }
  }
  return states;
}

// DONE: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid)
{
  string line;
  string filename = kProcDirectory + "/" + std::to_string(pid) + kStatusFilename;
  std::ifstream stream(filename);
  while (stream.is_open() and !stream.eof())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    string name, value;
    linestream >> name >> value;
    if (name.compare("Uid:") == 0)
    {
      return value;
    }
  }
  return string();
}

// DONE: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid)
{
  string target_uid = Uid(pid);

  string line;
  std::ifstream stream(kPasswordPath);
  while (stream.is_open() and !stream.eof())
  {
    std::getline(stream, line);
    std::replace(line.begin(), line.end(), ':', ' ');
    std::istringstream linestream(line);
    string name, x, uid;
    linestream >> name >> x >> uid;
    if (uid == target_uid)
    {
      return name;
    }
  }

  return string();
}

// DONE: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid)
{
  auto states = CpuUtilization(pid);
  float starttime = stof(states[ProcessCPUStates::kStartTime_]) / sysconf(_SC_CLK_TCK);
  return (long)starttime;
}