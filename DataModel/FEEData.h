#ifndef FEEDATA_H
#define FEEDATA_H

#include <iostream>
#include <vector>

class FEEData{


 public:

  FEEData();
  bool Send();
  bool Receive();

  std::vector<int> Data;
  long Time;
  int CardID;
  int ChannelID;

 private:




};


#endif
