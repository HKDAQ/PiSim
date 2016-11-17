#include "BrokerEBUReceiver.h"

BrokerEBUReceiver::BrokerEBUReceiver():Tool(){}


bool BrokerEBUReceiver::Initialise(std::string configfile, DataModel &data){

  if(configfile!="")  m_variables.Initialise(configfile);
  //m_variables.Print();

  m_data= &data;

  trigreceive = new zmq::socket_t(*(m_data->context), ZMQ_SUB);
  trigreceive->setsockopt(ZMQ_SUBSCRIBE,"",0);
  trigreceive->connect("tcp://localhost:55555");

  return true;
}


bool BrokerEBUReceiver::Execute(){


  zmq::message_t comm;
  //  std::cout<<"waiting for data"<<std::endl;
  trigreceive->recv(&comm);

  std::istringstream iss(static_cast<char*>(comm.data()));

  std::cout<<"message received = "<<iss.str()<<std::endl;

  TriggerData* tmp = new TriggerData();
  std::string dump;
  iss>>dump>>tmp->Time>>tmp->CardID>>tmp->ChannelID;
  m_data->UnSortedTrigData.push_back(tmp);

  return true;
}


bool BrokerEBUReceiver::Finalise(){

  return true;
}
