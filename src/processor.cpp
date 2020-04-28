#include "processor.h"
#include "linux_parser.h"

void Processor::Update() {
  LinuxParser::ProcessorState state = LinuxParser::CpuUtilization();

  long idle = state.idle - this->idle;
  long nonIdle = state.nonIdle - this->nonIdle;

  float utilization = float(nonIdle) / float(idle + nonIdle);

  this->idle = state.idle;
  this->nonIdle = state.nonIdle;
  this->utilization = utilization;
}

float Processor::GetUtilization() { return utilization; }