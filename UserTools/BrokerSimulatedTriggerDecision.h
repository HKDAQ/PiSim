#ifndef BrokerSimulatedTriggerDecision_H
#define BrokerSimulatedTriggerDecision_H

#include <string>
#include <iostream>

#include "Tool.h"

class BrokerSimulatedTriggerDecision: public Tool {


 public:

  BrokerSimulatedTriggerDecision();
  bool Initialise(std::string configfile,DataModel &data);
  bool Execute();
  bool Finalise();


 private:





};


#endif
