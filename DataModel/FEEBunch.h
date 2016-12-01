#ifndef FEEBUNCH_H
#define FEEBUNCH_H

#include <iostream>
#include <vector>

#include <zmq.hpp>
#include <FEEData.h>

class FEEBunch{


 public:

  FEEBunch();
  ~FEEBunch();
  bool Send(zmq::socket_t &sock, bool sendmore=false);
  bool Receive(zmq::socket_t &sock);

  std::vector<FEEData*> Data;
  long Time;

 private:




};


#endif
