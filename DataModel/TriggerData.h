#ifndef TRIGGERDATA_H
#define TRIGGERDATA_H

#include <iostream>
#include <vector>

class TriggerData{


 public:

  TriggerData();
  bool Send();
  bool Receive();

  long Time;
  int CardID;
  int ChannelID;

 private:




};


#endif
