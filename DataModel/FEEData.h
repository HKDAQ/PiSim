#ifndef FEEDATA_H
#define FEEDATA_H

#include <iostream>
#include <vector>

#include <zmq.hpp>
#include <FEEChData.h>

class FEEData{


 public:

  FEEData();
  ~FEEData();
  bool Send(zmq::socket_t &sock, bool sendmore=false);
  bool Receive(zmq::socket_t &sock);

  std::vector<FEEChData*> Data;
  long Time;

 private:




};


#endif
