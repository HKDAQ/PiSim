#ifndef TRIGGERBUNCH_H
#define TRIGGERBUNCH_H

#include <iostream>
#include <vector>
#include <TriggerData.h>

#include <zmq.hpp>

class TriggerBunch{


 public:

  TriggerBunch();
  ~TriggerBunch();
  bool Send(zmq::socket_t &sock);
  bool Receive(zmq::socket_t &sock);

  std::vector<TriggerData*> Data;
  long StartTime;
  long EndTime;
  bool Decision;

 private:




};


#endif
