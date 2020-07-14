#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>

class Processor
{
public:
    float Utilization(); // TODO: See src/processor.cpp
    Processor() : cores{0}, model_name(""){};
    Processor(int cores, std::string model_name) : cores{cores}, model_name(model_name){};

    // TODO: Declare any necessary private members
private:
    int cores;
    std::string model_name;
};

#endif