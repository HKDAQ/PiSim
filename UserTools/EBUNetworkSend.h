#ifndef EBUNetworkSend_H
#define EBUNetworkSend_H

#include <string>
#include <iostream>
#include <pthread.h>

#include <zmq.hpp>

#include "Tool.h"

struct context_args{

  context_args(zmq::context_t* incontext){

    context=incontext;
  }

  zmq::context_t* context;
  
};


class EBUNetworkSend: public Tool {


 public:

  EBUNetworkSend();
  bool Initialise(std::string configfile,DataModel &data);
  bool Execute();
  bool Finalise();


 private:

  zmq::socket_t* Isend;
  
  pthread_t thread[3];

  static void *ProcessorThread(void* arg);
  static void *TriggerThread(void* arg);
  static void *FullDataThread(void* arg);

  context_args* args;

};


#endif
