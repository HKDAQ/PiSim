#ifndef RBUFEEReceiverSim_H
#define RBUFEEReceiverSim_H

#include <string>
#include <iostream>
#include <stdio.h>

#include "Tool.h"

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <vector>
#include <sys/time.h>

class RBUFEEReceiverSim: public Tool {


 public:

  RBUFEEReceiverSim();
  bool Initialise(std::string configfile,DataModel &data);
  bool Execute();
  bool Finalise();


 private:

  long simtime;
  int sleeptime;
  int cards;
  int channels;
  int prob;
  int buffersize;

};


#endif
