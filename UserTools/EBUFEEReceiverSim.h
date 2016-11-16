#ifndef EBUFEEReceiverSim_H
#define EBUFEEReceiverSim_H

#include <string>
#include <iostream>
#include <stdio.h>

#include "Tool.h"

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <vector>

class EBUFEEReceiverSim: public Tool {


 public:

  EBUFEEReceiverSim();
  bool Initialise(std::string configfile,DataModel &data);
  bool Execute();
  bool Finalise();


 private:

  long simtime;

};


#endif
