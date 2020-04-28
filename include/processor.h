#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  /** Updates state of prcessor. */
  void Update();
  /**
   * Calculated processor utilization. Value represents utilization of
   * multi-core CPU during in the time range between two recent consecutive
   * calls of Update method.
   * */
  float GetUtilization();

 private:
  float utilization = 0;
  long nonIdle = 0;  // Handles previous NON-IDLE value
  long idle = 0;     // Handles previous IDLE value
};

#endif