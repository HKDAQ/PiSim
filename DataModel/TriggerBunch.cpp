#include "TriggerBunch.h"

TriggerBunch::TriggerBunch(){

}

TriggerBunch::~TriggerBunch(){

  for (int i=0; i<Data.size(); i++){
    delete Data.at(i);
    Data.at(i)=0;
  }
  Data.clear();
}

bool TriggerBunch::Send(zmq::socket_t &sock){
  long tmp=Data.size();
  zmq::message_t msg1(&tmp,sizeof tmp,NULL);
  sock.send(msg1,ZMQ_SNDMORE);

  std::cout<<"sending size "<<tmp<<std::endl;
  if (tmp!=400) std::cout<<std::endl<<std::endl<<std::endl<<std::endl<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11"<<std::endl<<std::endl<<std::endl<<std::endl;
 
 for(int i=0; i<Data.size();i++){
   Data.at(i)->Send(sock,true);
  }

 zmq::message_t msg2(&StartTime,sizeof StartTime, NULL);
 zmq::message_t msg3(&EndTime, sizeof EndTime, NULL);
 zmq::message_t msg4(&Decision, sizeof Decision, NULL);

 sock.send(msg2,ZMQ_SNDMORE);
 sock.send(msg3,ZMQ_SNDMORE);
 sock.send(msg4);

 std::cout<<"sending trig"<<std::endl;

  return true;

}

bool TriggerBunch::Receive(zmq::socket_t &sock){

  std::cout<<"in receive"<<std::endl;

  zmq::message_t msg1;
  sock.recv(&msg1);
  long size=*(reinterpret_cast<long*>(msg1.data()));
  std::cout<<"received first "<<size<<std::endl;  

  for (int i=0;i<size; i++){
    TriggerData* tmp=new TriggerData();
    tmp->Receive(sock);
    Data.push_back(tmp);
  }

  zmq::message_t msg2;
  sock.recv(&msg2);
  StartTime=*(reinterpret_cast<long*>(msg2.data()));

  zmq::message_t msg3;
  sock.recv(&msg3);
  EndTime=*(reinterpret_cast<long*>(msg3.data()));
  
  zmq::message_t msg4;
  sock.recv(&msg4);
  Decision=*(reinterpret_cast<bool*>(msg4.data()));


  return true;

}
