#include "FEEData.h"


FEEData::FEEData(){

}

FEEData::~FEEData(){

  for(int i=0; i<Data.size(); i++){
    delete Data.at(i);
    Data.at(i)=0;
  }
  Data.clear();
}

bool FEEData::Send(zmq::socket_t &sock, bool sendmore){

  return true;

}

bool FEEData::Receive(zmq::socket_t &sock){

  return true;

}
