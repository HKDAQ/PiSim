#include "TriggerData.h"

void bencleanup(void *data, void *hint){

}

TriggerData::TriggerData(){



}

bool TriggerData::Send(zmq::socket_t &sock, bool sendmore){

  zmq::message_t msg1(&Time, sizeof Time, bencleanup);
  zmq::message_t msg2(&CardID, sizeof CardID, bencleanup);
  zmq::message_t msg3(&ChannelID, sizeof ChannelID, bencleanup);

  sock.send(msg1,ZMQ_SNDMORE);
  sock.send(msg2,ZMQ_SNDMORE);
  if(sendmore) sock.send(msg3,ZMQ_SNDMORE);
  else sock.send(msg3);

  return true;

}

bool TriggerData::Receive(zmq::socket_t &sock){

  zmq::message_t msg1;
  sock.recv(&msg1);
  Time=*(reinterpret_cast<long*>(msg1.data()));

  zmq::message_t msg2;
  sock.recv(&msg2);
  CardID=*(reinterpret_cast<int*>(msg2.data()));

  zmq::message_t msg3;
  sock.recv(&msg3);
  ChannelID=*(reinterpret_cast<int*>(msg3.data()));

  return true;

}
