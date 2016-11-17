#ifndef TRIGGERBUNCH_H
#define TRIGGERBUNCH_H

#include <iostream>
#include <vector>
#include <TriggerData.h>

class TriggerBunch{


 public:

  TriggerBunch();
  bool Send();
  bool Receive();

  std::vector<TriggerData*> Data;
  long StartTime;
  long EndTime;
  bool Decision;

 private:




};


#endif
