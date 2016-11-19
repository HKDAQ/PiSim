#ifndef RBUSNTrigger_H
#define RBUSNTrigger_H

#include <string>
#include <iostream>

#include "Tool.h"

#include <zmq.hpp>

class RBUSNTrigger: public Tool {


 public:

  RBUSNTrigger();
  bool Initialise(std::string configfile,DataModel &data);
  bool Execute();
  bool Finalise();


 private:

  zmq::socket_t* SNReceive;



};


#endif
