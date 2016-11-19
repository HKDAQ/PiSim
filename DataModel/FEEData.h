#ifndef FEEDATA_H
#define FEEDATA_H

#include <iostream>
#include <vector>

#include <zmq.hpp>

class FEEData{


 public:

  FEEData();
  bool Send(zmq::socket_t &sock, bool sendmore=false);
  bool Receive(zmq::socket_t &sock);

  std::vector<int> Data;
  long Time;
  int CardID;
  int ChannelID;

 private:




};


#endif
