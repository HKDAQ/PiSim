#ifndef BrokerRBUReceiver_H
#define BrokerRBUReceiver_H

#include <string>
#include <iostream>

#include "Tool.h"

#include <zmq.hpp>

class BrokerRBUReceiver: public Tool {


 public:

  BrokerRBUReceiver();
  bool Initialise(std::string configfile,DataModel &data);
  bool Execute();
  bool Finalise();


 private:

  zmq::socket_t* trigreceive;



};


#endif
