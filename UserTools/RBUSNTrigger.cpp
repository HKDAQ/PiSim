#include "RBUSNTrigger.h"

RBUSNTrigger::RBUSNTrigger():Tool(){}


bool RBUSNTrigger::Initialise(std::string configfile, DataModel &data){

  if(configfile!="")  m_variables.Initialise(configfile);
  //m_variables.Print();

  m_data= &data;
  
  std::string ip;
  int port;

  m_variables.Get("IP", ip);
  m_variables.Get("Port", port);

  std::stringstream connection;
  connection<<"tcp://"<<ip<<":"<<port;

  SNReceive= new zmq::socket_t(*(m_data->context), ZMQ_SUB);
  SNReceive->setsockopt(ZMQ_SUBSCRIBE,"",0);
  SNReceive->connect(connection.str().c_str());

  return true;
}


bool RBUSNTrigger::Execute(){

  zmq::message_t* comm= new zmq::message_t();
  if(SNReceive->recv(comm, ZMQ_NOBLOCK)){
    
    std::istringstream iss(static_cast<char*>(comm->data()));
    
    if(iss.str().c_str()=="SN Start") m_data->SNTrigger=true;
    else if(iss.str().c_str()=="SN Stop") m_data->SNTrigger=false;
    
  }
  
  
  return true;
}


bool RBUSNTrigger::Finalise(){

  return true;
}
