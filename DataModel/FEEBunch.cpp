#include "FEEBunch.h"


FEEBunch::FEEBunch(){

}

FEEBunch::~FEEBunch(){
  for(int i=0; i<Data.size(); i++){
    delete Data.at(i);
    Data.at(i)=0;
  }
  Data.clear();
}

bool FEEBunch::Send(zmq::socket_t &sock, bool sendmore){

  return true;

}

bool FEEBunch::Receive(zmq::socket_t &sock){

  return true;

}
