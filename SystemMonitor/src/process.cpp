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

// DONE: Return this process's ID
int Process::Pid() { return pid; }

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() const
{
    auto states = LinuxParser::CpuUtilization(pid);
    float utime = stof(states[LinuxParser::ProcessCPUStates::kUTime_]);
    float stime = stof(states[LinuxParser::ProcessCPUStates::kSTime_]);
    float starttime = stof(states[LinuxParser::ProcessCPUStates::kStartTime_]) / sysconf(_SC_CLK_TCK);

    float total_time = (utime + stime) / sysconf(_SC_CLK_TCK);
    float sys_uptime = LinuxParser::UpTime();
    float elapsed = sys_uptime - starttime;
    return total_time / elapsed;
}

// DONE: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid); }

// DONE: Return this process's memory utilization
string Process::Ram()
{
    int ram_kb = stoi(LinuxParser::Ram(pid));
    if (ram_kb < 1000)
    {
        // avoid outputting 0 MB
        string ram_mb = std::to_string(ram_kb / 1000.0f);
        return ram_mb.substr(0, ram_mb.find(".") + 3);
    }
    else
    {
        string ram_mb = std::to_string(ram_kb / 1000);
        return ram_mb;
    }
}

// DONE: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid); }

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid); }

// DONE: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const &a) const
{
    return a.CpuUtilization() < this->CpuUtilization();
}