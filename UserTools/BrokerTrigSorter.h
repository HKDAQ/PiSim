#ifndef BrokerTrigSorter_H
#define BrokerTrigSorter_H

#include <string>
#include <iostream>

#include "Tool.h"

class BrokerTrigSorter: public Tool {


 public:

  BrokerTrigSorter();
  bool Initialise(std::string configfile,DataModel &data);
  bool Execute();
  bool Finalise();


 private:

  long lasttimeslice;
  



};


#endif
