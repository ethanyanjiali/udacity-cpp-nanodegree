#include <string>

#include "processor.h"
#include "linux_parser.h"

// DONE: Return the aggregate CPU utilization
float Processor::Utilization()
{
    auto cpu_states = LinuxParser::CpuUtilization();
    float idle = stof(cpu_states[LinuxParser::CPUStates::kIdle_]) + stof(cpu_states[LinuxParser::CPUStates::kIOwait_]);
    float user = stof(cpu_states[LinuxParser::CPUStates::kUser_]);
    float nice = stof(cpu_states[LinuxParser::CPUStates::kNice_]);
    float system = stof(cpu_states[LinuxParser::CPUStates::kSystem_]);
    float irq = stof(cpu_states[LinuxParser::CPUStates::kIRQ_]);
    float softirq = stof(cpu_states[LinuxParser::CPUStates::kSoftIRQ_]);
    float steal = stof(cpu_states[LinuxParser::CPUStates::kSteal_]);
    float non_idle = user + nice + system + irq + softirq + steal;
    return non_idle / (idle + non_idle);
}