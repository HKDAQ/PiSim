#ifndef TRIGGERDATA_H
#define TRIGGERDATA_H

#include <iostream>
#include <vector>

#include <zmq.hpp>

class TriggerData{


 public:

  TriggerData();
  bool Send(zmq::socket_t &sock);
  bool Receive(zmq::socket_t &sock);

  long Time;
  int CardID;
  int ChannelID;

 private:




};


#endif
