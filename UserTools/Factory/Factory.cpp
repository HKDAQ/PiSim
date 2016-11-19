#include "../Unity.cpp"

Tool* Factory(std::string tool){
Tool* ret=0;

// if (tool=="Type") tool=new Type;
if (tool=="DummyTool") ret=new DummyTool;


if (tool=="RBUSNTrigger") ret=new RBUSNTrigger;
if (tool=="RBUFEEReceiverSim") ret=new RBUFEEReceiverSim;
if (tool=="RBUNetworkSend") ret=new RBUNetworkSend;
if (tool=="BrokerRBUReceiver") ret=new BrokerRBUReceiver;
if (tool=="BrokerTrigSorter") ret=new BrokerTrigSorter;
  if (tool=="BrokerSimulatedTriggerDecision") ret=new BrokerSimulatedTriggerDecision;
return ret;
}

