#ifndef BrokerEBUReceiver_H
#define BrokerEBUReceiver_H

#include <string>
#include <iostream>

#include "Tool.h"

#include <zmq.hpp>

class BrokerEBUReceiver: public Tool {


 public:

  BrokerEBUReceiver();
  bool Initialise(std::string configfile,DataModel &data);
  bool Execute();
  bool Finalise();


 private:

  zmq::socket_t* trigreceive;



};


#endif
