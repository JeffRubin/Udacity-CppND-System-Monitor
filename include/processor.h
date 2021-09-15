#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "updateable.h"

class Processor : public Updateable {
 public:
  void Update() override;
  float Utilization() const { return utilization_; }

 private:
  float utilization_{0.0};
  unsigned long long prev_nonidle_{0};
  unsigned long long prev_total_{0};
};

#endif