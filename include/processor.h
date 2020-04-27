#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();

 private:
  long nonIdle = 0;
  long idle = 0;
};

#endif