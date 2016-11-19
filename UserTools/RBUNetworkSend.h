#ifndef RBUNetworkSend_H
#define RBUNetworkSend_H

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
  int triggerpubport;
  int fulldataport;
  int bufferdeletetimeout;
  int bufferdeleteperiod;
  int subcatscanperiod;
};


class RBUNetworkSend: public Tool {


 public:

  RBUNetworkSend();
  bool Initialise(std::string configfile,DataModel &data);
  bool Execute();
  bool Finalise();


 private:

  zmq::socket_t* Isend;
  zmq::socket_t* subcat;
  
  pthread_t thread[4];

  static void *ProcessorThread(void* arg);
  static void *TriggerThread(void* arg);
  static void *FullDataThread(void* arg);
  static void *SubCatThread(void* arg);

  context_args* args;

};


#endif
